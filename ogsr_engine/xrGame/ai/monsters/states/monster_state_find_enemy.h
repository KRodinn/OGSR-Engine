#pragma once

#include "../state.h"


template<typename _Object>
class	CStateMonsterFindEnemy : public CState<_Object> {
protected:
	typedef CState<_Object>		inherited;
	typedef CState<_Object>*	state_ptr;

public:
						CStateMonsterFindEnemy	(_Object *obj);
	virtual				~CStateMonsterFindEnemy	();

	virtual	void		reselect_state			();
	virtual void		remove_links					(CObject* object) { inherited::remove_links(object);}
};

#include "monster_state_find_enemy_inline.h"

