#pragma once

enum EUIMessages : s16
{
	// CUIWindow
	WINDOW_LBUTTON_DOWN = 0,
	WINDOW_RBUTTON_DOWN,
	WINDOW_CBUTTON_DOWN,

	WINDOW_LBUTTON_UP,
	WINDOW_RBUTTON_UP,
	WINDOW_CBUTTON_UP,

	WINDOW_MOUSE_MOVE,
	WINDOW_MOUSE_WHEEL_UP,
	WINDOW_MOUSE_WHEEL_DOWN,
	WINDOW_LBUTTON_DB_CLICK,
	WINDOW_KEY_PRESSED,
	WINDOW_KEY_RELEASED,
	WINDOW_MOUSE_CAPTURE_LOST, 
	WINDOW_KEYBOARD_CAPTURE_LOST,

	// CUIStatic
	STATIC_FOCUS_RECEIVED,
	STATIC_FOCUS_LOST,

	// CUIButton
	BUTTON_CLICKED,
	BUTTON_DOWN,

	// CUITabControl
	TAB_CHANGED,	

	// CUICheckButton
	CHECK_BUTTON_SET,
	CHECK_BUTTON_RESET, 

	// CUIRadioButton
	RADIOBUTTON_SET,

	// CUIdragDropItem
	DRAG_DROP_ITEM_DRAG,
	DRAG_DROP_ITEM_DROP, 
	DRAG_DROP_ITEM_DB_CLICK,
	DRAG_DROP_ITEM_RBUTTON_CLICK,
	DRAG_DROP_ITEM_SELECTED,
	
	// CUIScrollBox
	SCROLLBOX_MOVE,

	// CUIScrollBar
	SCROLLBAR_VSCROLL,
	SCROLLBAR_HSCROLL,
	SCROLLBAR_NEEDUPDATE,

	// CUIScrollView
	CHILD_CHANGED_SIZE,

	// CUIListWnd
	LIST_ITEM_CLICKED,
	LIST_ITEM_DB_CLICKED,
	LIST_ITEM_SELECT,
	LIST_ITEM_UNSELECT,
	LIST_ITEM_FOCUS_RECEIVED,

	// UIPropertiesBox
	PROPERTY_CLICKED,

	// CUIMessageBox
	MESSAGE_BOX_OK_CLICKED,
	MESSAGE_BOX_YES_CLICKED,
	MESSAGE_BOX_QUIT_WIN_CLICKED,
	MESSAGE_BOX_QUIT_GAME_CLICKED,
	MESSAGE_BOX_NO_CLICKED,
	MESSAGE_BOX_CANCEL_CLICKED,

	// CUITalkDialogWnd
	TALK_DIALOG_TRADE_BUTTON_CLICKED,
	TALK_DIALOG_QUESTION_CLICKED,

	// CUIPdaWnd
	PDA_OPEN_ENCYCLOPEDIA_ARTICLE,
	PDA_ENCYCLOPEDIA_HAS_ARTICLE,

	// CUIPdaContactsWnd
	PDA_CONTACTS_WND_CONTACT_SELECTED,

	// CUITradeWnd
	TRADE_WND_CLOSED,

	// CUIInventroyWnd
	INVENTORY_DROP_ACTION,
	INVENTORY_EAT_ACTION,
	INVENTORY_TO_BELT_ACTION, 
	INVENTORY_TO_SLOT_ACTION,
	INVENTORY_TO_BAG_ACTION,
	INVENTORY_ATTACH_ADDON, 

	INVENTORY_ATTACH_SCOPE_ADDON,
	INVENTORY_DETACH_SCOPE_ADDON,

    INVENTORY_ATTACH_SILENCER_ADDON,	
	INVENTORY_DETACH_SILENCER_ADDON,

	INVENTORY_ATTACH_GRENADE_LAUNCHER_ADDON,
	INVENTORY_DETACH_GRENADE_LAUNCHER_ADDON,

	INVENTORY_RELOAD_MAGAZINE,
	INVENTORY_UNLOAD_MAGAZINE,
	INVENTORY_SELL_ITEM,

	INVENTORY_MOVE_ACTION,

	// BuyWeaponWnd
	XR_MENU_LEVEL_CHANGED,
	MP_MONEY_CHANGE,

	//CUIAnimationBase
    ANIMATION_STOPPED,
	EDIT_TEXT_CHANGED,
	EDIT_TEXT_COMMIT,
	MAP_SHOW_HINT,
	MAP_HIDE_HINT,

	MAP_SELECT_SPOT2,
	MAP_REMOVE_SPOT_ACT,
	MAP_CHANGE_SPOT_HINT_ACT,
};
