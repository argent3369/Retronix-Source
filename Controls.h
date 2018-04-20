/*

*/

#pragma once

#include "GUI.h"

class CCheckBox : public CControl
{
public:
	CCheckBox();
	void SetState(bool s);
	bool GetState();
	bool GetIsSub();
	void SetAsSub(bool t);
protected:
	bool bIsSub;
	bool Checked;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
	bool last_state;
	bool should_animate;
	int animation_state;
};
class CSeperate : public CControl
{
public:
	CSeperate();
protected:
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CLabel : public CControl
{
public:
	CLabel();
	void SetText(std::string text);
protected:
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CGroupTab
{
public:
	std::string name;
	int id;
public:
	CGroupTab(std::string _name, int _id)
	{
		this->name = _name;
		this->id = _id;
	}
};
class CGroupBox : public CControl
{
public:
	CGroupBox();
	void SetText(std::string text);
	void PlaceComboBox(int g_tab, std::string Label, CTab * Tab, CControl * control);
	//void PlaceLabledControl(std::string Label, CTab *Tab, CControl* control);
	void PlaceLabledControl(int g_tab, std::string Label, CTab * Tab, CControl * control);
	void PlaceLabledControl33(int g_tab, std::string Label, CTab * Tab, CControl * control);
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Items;
	std::string Text;
	int last_y;
	int last_control_height;
	float iYAdd;
	int ItemSpacing;
	bool initialized = false;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};


class CGroupBoxColorItems : public CControl
{
public:
	CGroupBoxColorItems();
	void SetText(std::string text);
	void PlaceLabledControl(int g_tab, std::string Label, CTab *Tab, CControl* control);
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Items;
	std::string Text;
	int last_y;
	int last_control_height;
	bool initialized = false;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CGroupBox2 : public CControl
{
public:
	CGroupBox2();
	void SetText(std::string text);
	void PlaceComboBox(int g_tab, std::string Label, CTab * Tab, CControl * control);
	//void PlaceLabledControl(std::string Label, CTab *Tab, CControl* control);
	void PlaceLabledControl(int g_tab, std::string Label, CTab * Tab, CControl * control);
	void PlaceLabledControl2(int g_tab, std::string Label, CTab * Tab, CControl * control);
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Items;
	std::string Text;
	int last_y;
	int last_control_height;
	float iYAdd;
	int ItemSpacing;
	bool initialized = false;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};


class CSlider : public CControl
{
public:
	CSlider();
	float GetValue();
	void SetValue(float v);
	void SetBoundaries(float min, float max);
protected:
	float Value;
	float Min;
	float Max;
	bool DoDrag;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CKeyBind : public CControl
{
public:
	CKeyBind();
	int GetKey();
	void SetKey(int key);
protected:
	bool Getting_New_Key;
	int key = -1;
	int Key;
	char* text;
	bool IsGettingKey;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButton : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButton();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButtonSave : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButtonSave();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButtonLoad : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButtonLoad();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButtonApplySkin : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButtonApplySkin();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CComboBox : public CControl
{
public:
	CComboBox();
	void AddItem(std::string text);
	void SelectIndex(int idx);
	int GetIndex();
	void SetIndex(int index);
	std::string GetItem();
protected:
	std::vector<std::string> Items;
	int SelectedIndex;
	bool IsOpen;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CListBox : public CControl
{
public:
	CListBox();
	void AddItem(std::string text, int value = 0);
	inline int GetIndex() { return SelectedIndex; }
	std::string GetItem();
	inline int GetValue() { if (SelectedIndex >= 0 && SelectedIndex < Items.size()) return Values[SelectedIndex]; else return -1; };
	inline void ClearItems() { Items.clear(); Values.clear(); }
	void SetHeightInItems(int items);
	inline void SelectItem(int idx) { if (idx >= 0 && idx < Items.size()) SelectedIndex = idx; }
private:
	std::vector<std::string> Items;
	std::vector<int> Values;

	int ScrollTop;
	int SelectedIndex;

	bool dragging;

	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CTextField : public CControl
{
public:
	CTextField();
	std::string getText();
	void SetText(std::string);
private:
	std::string text;
	bool IsGettingKey;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CSilentGroupBox : public CControl
{
public:
	CSilentGroupBox();
	void SetText(std::string text);
	void PlaceLabledControl(std::string Label, CTab *Tab, CControl* control);
protected:
	int Items;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};


struct MultiBoxItem
{
	bool bselected;
	const char* text;

	MultiBoxItem(bool _bselected, const char* _text)
	{
		this->bselected = _bselected;
		this->text = _text;
	}
};

class CMultiBox : public CControl
{
public:
	CMultiBox();
	std::vector<MultiBoxItem> items;
	void SetTitle(const char*);
private:
	const char* title;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};