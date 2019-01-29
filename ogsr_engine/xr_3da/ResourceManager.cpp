// TextureManager.cpp: implementation of the CResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(default:4995)

#include "ResourceManager.h"
#include "tss.h"
#include "blenders\blender.h"
#include "blenders\blender_recorder.h"


ENGINE_API void fix_texture_name(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if(  _ext					&&
	  (0==stricmp(_ext,".tga")	||
		0==stricmp(_ext,".dds")	||
		0==stricmp(_ext,".bmp")	||
		0==stricmp(_ext,".ogm")	) )
		*_ext = 0;
}
//--------------------------------------------------------------------------------------------------------------
template <class T>
BOOL	reclaim		(xr_vector<T*>& vec, const T* ptr)
{
	xr_vector<T*>::iterator it	= vec.begin	();
	xr_vector<T*>::iterator end	= vec.end	();
	for (; it!=end; it++)
		if (*it == ptr)	{ vec.erase	(it); return TRUE; }
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------
IBlender* CResourceManager::_GetBlender		(LPCSTR Name)
{
	R_ASSERT(Name && Name[0]);

	LPSTR N = LPSTR(Name);
	map_Blender::iterator I = m_blenders.find	(N);
	if (I==m_blenders.end())	{ Debug.fatal(DEBUG_INFO,"Shader '%s' not found in library.",Name); return 0; }
	else					return I->second;
}

IBlender* CResourceManager::_FindBlender		(LPCSTR Name)
{
	if (!(Name && Name[0])) return 0;

	LPSTR N = LPSTR(Name);
	map_Blender::iterator I = m_blenders.find	(N);
	if (I==m_blenders.end())	return 0;
	else						return I->second;
}

xr_vector<CTexture*> CResourceManager::_FindTexture(LPCSTR Name)
{
	R_ASSERT(Name && Name[0]);

	string_path	filename;
	strcpy_s(filename, Name); //. andy if (strext(Name)) *strext(Name)=0;
	fix_texture_name(filename);

	LPSTR N = LPSTR(filename);
	char *ch = strstr(N, "*");

	xr_vector<CTexture*> r;

	if (NULL == ch) // no wildcard?
	{
		map_TextureIt I = m_textures.find(N);
		if (I != m_textures.end())	
			r.push_back(I->second);
	}
	else
	{
		// alpet: test for wildcard matching
		ch[0] = 0; // remove *

		for (map_TextureIt t = m_textures.begin(); t != m_textures.end(); t++)
			if (strstr(t->second->cName.c_str(), N))
				r.push_back(t->second);
	}

	return r;
}

void	CResourceManager::ED_UpdateBlender	(LPCSTR Name, IBlender* data)
{
	LPSTR N = LPSTR(Name);
	map_Blender::iterator I = m_blenders.find	(N);
	if (I!=m_blenders.end())	{
		R_ASSERT	(data->getDescription().CLS == I->second->getDescription().CLS);
		xr_delete	(I->second);
		I->second	= data;
	} else {
		m_blenders.insert	(mk_pair(xr_strdup(Name),data));
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void	CResourceManager::_ParseList(sh_list& dest, LPCSTR names)
{
	if (!names || !names[0])
		names = "$null";

	dest.clear(); // intorr: To avoid memory corruption with debug runtime.
	char*	P			= (char*) names;
	svector<char,128>	N;

	while (*P)
	{
		if (*P == ',') {
			// flush
			N.push_back	(0);
			strlwr		(N.begin());

			fix_texture_name( N.begin() );
			dest.push_back(N.begin());
			N.clear		();
		} else {
			N.push_back	(*P);
		}
		P++;
	}
	if (N.size())
	{
		// flush
		N.push_back	(0);
		strlwr		(N.begin());

		fix_texture_name( N.begin() );
		dest.push_back(N.begin());
	}
}

ShaderElement* CResourceManager::_CreateElement			(ShaderElement& S)
{
	if (S.passes.empty())		return	0;

	// Search equal in shaders array
	for (u32 it=0; it<v_elements.size(); it++)
		if (S.equal(*(v_elements[it])))	return v_elements[it];

	// Create _new_ entry
	ShaderElement*	N		=	xr_new<ShaderElement>(S);
	N->dwFlags				|=	xr_resource_flagged::RF_REGISTERED;
	v_elements.push_back	(N);
	return N;
}

void CResourceManager::_DeleteElement(const ShaderElement* S)
{
	if (0==(S->dwFlags&xr_resource_flagged::RF_REGISTERED))	return;
	if (reclaim(v_elements,S))						return;
	Msg	("! ERROR: Failed to find compiled 'shader-element'");
}

Shader*	CResourceManager::_cpp_Create	(IBlender* B, LPCSTR s_shader, LPCSTR s_textures, LPCSTR s_constants, LPCSTR s_matrices)
{
	CBlender_Compile	C;
	Shader				S;

	//.
	// if (strstr(s_shader,"transparent"))	DEBUG_INVOKE;

	// Access to template
	C.BT				= B;
	C.bEditor			= FALSE;
	C.bDetail			= FALSE;

	// Parse names
	_ParseList			(C.L_textures,	s_textures	);
	_ParseList			(C.L_constants,	s_constants	);
	_ParseList			(C.L_matrices,	s_matrices	);

	// Compile element	(LOD0 - HQ)
	{
		C.iElement			= 0;
		C.bDetail			= m_textures_description.GetDetailTexture(C.L_textures[0],C.detail_texture,C.detail_scaler);
//.		C.bDetail			= _GetDetailTexture(*C.L_textures[0],C.detail_texture,C.detail_scaler);
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[0]				= _CreateElement	(E);
	}

	// Compile element	(LOD1)
	{
		C.iElement			= 1;
//.		C.bDetail			= _GetDetailTexture(*C.L_textures[0],C.detail_texture,C.detail_scaler);
		C.bDetail			= m_textures_description.GetDetailTexture(C.L_textures[0],C.detail_texture,C.detail_scaler);
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[1]				= _CreateElement	(E);
	}

	// Compile element
	{
		C.iElement			= 2;
		C.bDetail			= FALSE;
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[2]				= _CreateElement	(E);
	}

	// Compile element
	{
		C.iElement			= 3;
		C.bDetail			= FALSE;
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[3]				= _CreateElement	(E);
	}

	// Compile element
	{
		C.iElement			= 4;
		C.bDetail			= TRUE;	//.$$$ HACK :)
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[4]				= _CreateElement	(E);
	}

	// Compile element
	{
		C.iElement			= 5;
		C.bDetail			= FALSE;
		ShaderElement		E;
		C._cpp_Compile		(&E);
		S.E[5]				= _CreateElement	(E);
	}

	// Search equal in shaders array
	for (u32 it=0; it<v_shaders.size(); it++)
		if (S.equal(v_shaders[it]))	return v_shaders[it];

	// Create _new_ entry
	Shader*		N			=	xr_new<Shader>(S);
	N->dwFlags				|=	xr_resource_flagged::RF_REGISTERED;
	v_shaders.push_back		(N);
	return N;
}

Shader*	CResourceManager::_cpp_Create	(LPCSTR s_shader, LPCSTR s_textures, LPCSTR s_constants, LPCSTR s_matrices)
{
	return	_cpp_Create(_GetBlender(s_shader?s_shader:"null"),s_shader,s_textures,s_constants,s_matrices);
}

Shader*		CResourceManager::Create	(IBlender*	B,		LPCSTR s_shader,	LPCSTR s_textures,	LPCSTR s_constants, LPCSTR s_matrices)
{
	return	_cpp_Create	(B,s_shader,s_textures,s_constants,s_matrices);
}

Shader*		CResourceManager::Create	(LPCSTR s_shader,	LPCSTR s_textures,	LPCSTR s_constants,	LPCSTR s_matrices)
{
  if ( _lua_HasShader( s_shader ) )
    return _lua_Create( s_shader, s_textures );
  else
    return _cpp_Create( s_shader, s_textures, s_constants, s_matrices );
}

void CResourceManager::Delete(const Shader* S)
{
	if (0==(S->dwFlags&xr_resource_flagged::RF_REGISTERED))	return;
	if (reclaim(v_shaders,S))						return;
	Msg	("! ERROR: Failed to find complete shader");
}


static xr_vector<CTexture*> textures_to_load;

static void LoadTextures(const size_t thread_num, const size_t textures_per_worker)
{
#ifdef DEBUG
	Msg("[%s] Thread [%u], amount of textures = [%u]", __FUNCTION__, thread_num, textures_per_worker);

	CTimer timer;
	timer.Start();
#endif
	const auto upperbound = thread_num * textures_per_worker;
	const auto lowerbound = upperbound - textures_per_worker;
	for (auto i = lowerbound; i < upperbound; i++)
	{
		if (i < textures_to_load.size())
			textures_to_load[i]->Load();
		else
			break;
	}
#ifdef DEBUG
	Msg("[%s] Thread [%u], texture loading time = [%u ms.]", __FUNCTION__, thread_num, timer.GetElapsed_ms());
#endif
}

void CResourceManager::DeferredUpload()
{
	if (Device.b_is_Ready)
	{
		CTimer timer;
		timer.Start();

		size_t nWorkers = 0;
		if ( strstr( Core.Params, "mt_texload" ) || psDeviceFlags.test( rsMTTexLoad ) )
		{
		nWorkers = TTAPI->threads.size();
		const size_t textures_per_worker = m_textures.size() / nWorkers;

		for (const auto& t : m_textures)
			textures_to_load.push_back(t.second);

		for (size_t i = 1; i <= nWorkers; ++i)
			TTAPI->threads[i - 1]->addJob([=] { LoadTextures(i, textures_per_worker); });

		TTAPI->wait();

		textures_to_load.clear();
		}
		else {
			for (const auto& pair : m_textures)
				pair.second->Load();
		}

		Msg( "[%s] texture loading time (%zi) using %u threads: [%.2f s.]", __FUNCTION__, m_textures.size(), nWorkers, timer.GetElapsed_sec() );
	}
}


/*
void	CResourceManager::DeferredUnload	()
{
	if (!Device.b_is_Ready)				return;
	for (map_TextureIt t=m_textures.begin(); t!=m_textures.end(); t++)
		t->second->Unload();
}
*/

void	CResourceManager::_GetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps)
{
	m_base=c_base=m_lmaps=c_lmaps=0;

	map_Texture::iterator I = m_textures.begin	();
	map_Texture::iterator E = m_textures.end	();
	for (; I!=E; I++)
	{
		u32 m = I->second->flags.MemoryUsage;
		if (strstr(I->first,"lmap"))
		{
			c_lmaps	++;
			m_lmaps	+= m;
		} else {
			c_base	++;
			m_base	+= m;
		}
	}
}
void	CResourceManager::_DumpMemoryUsage		()
{
	xr_multimap<u32,std::pair<u32,shared_str> >		mtex	;

	// sort
	{
		map_Texture::iterator I = m_textures.begin	();
		map_Texture::iterator E = m_textures.end	();
		for (; I!=E; I++)
		{
			u32			m = I->second->flags.MemoryUsage;
			shared_str	n = I->second->cName;
			mtex.insert (mk_pair(m,mk_pair(I->second->dwReference,n) ));
		}
	}

	// dump
	{
		xr_multimap<u32,std::pair<u32,shared_str> >::iterator I = mtex.begin	();
		xr_multimap<u32,std::pair<u32,shared_str> >::iterator E = mtex.end		();
		for (; I!=E; I++)
			Msg			("* %4.1f : [%4d] %s",float(I->first)/1024.f, I->second.first, I->second.second.c_str());
	}
}

void	CResourceManager::Evict()
{
	CHK_DX	(HW.pDevice->EvictManagedResources());
}
/*
BOOL	CResourceManager::_GetDetailTexture(LPCSTR Name,LPCSTR& T, R_constant_setup* &CS)
{
	LPSTR N = LPSTR(Name);
	map_TD::iterator I = m_td.find	(N);
	if (I!=m_td.end())
	{
		T	= I->second.T;
		CS	= I->second.cs;
		return TRUE;
	} else {
		return FALSE;
	}
}*/
