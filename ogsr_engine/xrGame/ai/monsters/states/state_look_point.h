#pragma once
#include "../state.h"
#include "state_data.h"

template<typename _Object>
class CStateMonsterLookToPoint : public CState<_Object> {
	typedef CState<_Object> inherited;

	SStateDataLookToPoint	data;

public:
						CStateMonsterLookToPoint	(_Object *obj);
	virtual				~CStateMonsterLookToPoint	();

	virtual void		initialize					();
	virtual	void		execute						();

	virtual bool		check_completion			();
	virtual void		remove_links					(CObject* object) { inherited::remove_links(object);}
};

#include "state_look_point_inline.h"