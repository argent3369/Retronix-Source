/*

*/

#include "Controls.h"
#include "RenderManager.h"

#pragma region Base Control
void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}

	return p;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions

#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iWidth = 10;
	m_iHeight = 10;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::GradientV(a.x, a.y, 13, 13, Color(75, 75, 75, 255), Color(75, 75, 75, 255));

	if (hover)
	{
		if (Checked)
		{
			Render::Text(a.x + 1, a.y, Color(0, 77, 233, 255), Render::Fonts::Menu, (L"\u2714"));
		}
		else
		{
			Render::Clear(a.x + 2, a.y + 2, 9, 9, Color(129, 129, 129, 255));
		}
		Render::Outline(a.x + 2, a.y + 2, 9, 9, Color(75, 75, 75, 80));
	}
	else if (Checked)
	{
		Render::Text(a.x + 1, a.y, Color(0, 77, 233, 255), Render::Fonts::Menu, (L"\u2714"));
		Render::Outline(a.x + 2, a.y + 2, 9, 9, Color(75, 75, 75, 80));
	}
}

void CCheckBox::OnUpdate() { m_iWidth = 10; m_iHeight = 10; }

void CCheckBox::OnClick()
{
	Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions

#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	FileControlType = UIC_Label;
	Text = "Default";
	FileIdentifier = "Default";
	m_iHeight = 10;
}

void CLabel::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Text2(a.x, a.y - 1, Text.c_str(), Render::Fonts::Menu, Color(205, 205, 205, 255));
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate() {}
void CLabel::OnClick() {}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	last_y = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
	FileControlType = UIControlTypes::UIC_GroupBox;
}

void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	Render::DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, 1));
	Render::Text2(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), Render::Fonts::Menu, Color(244, 244, 244, 255));
	if (group_tabs.size())
	{
		Render::Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(48, 48, 48, 255));
		Render::Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(48, 48, 48, 255));
		Render::DrawRect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(21, 21, 19, 255));
		Render::DrawRect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(62, 62, 62, 255));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = Render::GetTextSize(Render::Fonts::Menu, group_tabs[i].name.c_str());

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}

			if (selected_tab == group_tabs[i].id)
			{
				Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(0, 77, 230, 255));
				Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(39, 39, 39, 255));
				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::Menu, Color(205, 205, 205, 245));
			}
			else if (selected_tab != group_tabs[i].id)
				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::Menu, Color(92, 92, 92, 255));
		}
	}

	//Top Left
	Render::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, 255));

	//Top Right
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 255));

	//Left
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(49, 49, 49, 255));
	Render::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 255));

	//Bottom
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	//Right
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(48, 48, 48, 255));
	Render::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 255));
}

void CGroupBox::SetText(std::string text)
{
	Text = text;
}
void CGroupBox::PlaceComboBox(int g_tab, std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 34;
	int y = m_y + Items * ItemSpacing + iYAdd;

	x += m_iWidth / 2 - 13;

	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Label.c_str());
	y = y + 10 - (txtSize.bottom / 2);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	x = m_x + 34;
	y = m_y + Items * ItemSpacing + iYAdd;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 25, ch);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	iYAdd += 6;
}

void CGroupBox::PlaceLabledControl(int g_tab, std::string Label, CTab *Tab, CControl* control)
{
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_keybind = control->FileControlType == UIControlTypes::UIC_KeyBind;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y && !is_keybind) {
		RECT label_size = Render::GetTextSize(Render::Fonts::Menu, Label.c_str());
		last_y += 14;
		y = last_y;
	}

	if (!is_keybind)
		last_control_height = control_height + 7;

	if (is_keybind || (is_color && Label == "")) {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	if (!is_keybind) {
		if (!is_color || is_color && Label != "")
			last_y += control_height + 7;
	}


}
void CGroupBox::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBox::OnUpdate() {}
void CGroupBox::OnClick() {}
#pragma endregion Implementations of the Group Box functions



#pragma region CGroupBox2
CGroupBox2::CGroupBox2()
{
	Items = 1;
	last_y = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
	FileControlType = UIControlTypes::UIC_GroupBox;
}

void CGroupBox2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	Render::DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, 1));
	Render::Text2(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), Render::Fonts::Menu, Color(244, 244, 244, 255));
	if (group_tabs.size())
	{
		Render::Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(48, 48, 48, 255));
		Render::Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(48, 48, 48, 255));
		Render::DrawRect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(21, 21, 19, 255));
		Render::DrawRect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(62, 62, 62, 255));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = Render::GetTextSize(Render::Fonts::Icon, group_tabs[i].name.c_str());

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}

			if (selected_tab == group_tabs[i].id)
			{
				Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(0, 77, 230, 255));
				Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(39, 39, 39, 255));
				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::Icon, Color(205, 205, 205, 245));
			}
			else if (selected_tab != group_tabs[i].id)
				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::Icon, Color(92, 92, 92, 255));
		}
	}

	//Top Left
	Render::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, 255));

	//Top Right
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 255));

	//Left
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(49, 49, 49, 255));
	Render::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 255));

	//Bottom
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	//Right
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(48, 48, 48, 255));
	Render::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 255));
}

void CGroupBox2::SetText(std::string text)
{
	Text = text;
}
void CGroupBox2::PlaceComboBox(int g_tab, std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 34;
	int y = m_y + Items * ItemSpacing + iYAdd;

	x += m_iWidth / 2 - 13;

	RECT txtSize = Render::GetTextSize(Render::Fonts::Icon, Label.c_str());
	y = y + 10 - (txtSize.bottom / 2);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	x = m_x + 34;
	y = m_y + Items * ItemSpacing + iYAdd;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 25, ch);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	iYAdd += 6;
}

void CGroupBox2::PlaceLabledControl2(int g_tab, std::string Label, CTab *Tab, CControl* control)
{
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_keybind = control->FileControlType == UIControlTypes::UIC_KeyBind;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y && !is_keybind) {
		RECT label_size = Render::GetTextSize(Render::Fonts::Menu, Label.c_str());
		last_y += 14;
		y = last_y;
	}

	if (!is_keybind)
		last_control_height = control_height + 7;

	if (is_keybind || (is_color && Label == "")) {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	if (!is_keybind) {
		if (!is_color || is_color && Label != "")
			last_y += control_height + 7;
	}


}
void CGroupBox2::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBox2::OnUpdate() {}
void CGroupBox2::OnClick() {}
#pragma endregion Implementations of the Group Box functions



#pragma region Sliders
CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::GradientV(a.x, a.y, m_iWidth, 7, Color(62, 62, 62, 255), Color(55, 55, 55, 245));

	float Ratio = (Value - Min) / (Max - Min);
	float Location = Ratio * m_iWidth;



	Render::GradientV(a.x, a.y, Location, 7, Color(0, 77, 230, 255), Color(0, 77, 230, 255));

	Render::Outline(a.x, a.y, m_iWidth, 7, Color(2, 2, 2, 255));

	char buffer[24];
	sprintf_s(buffer, "%.f", Value);
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::Text2(a.x + Location - (txtSize.right / 2), a.y + 7 - (txtSize.bottom / 2), buffer, Render::Fonts::MenuBold, Color(180, 180, 180, 245));
}
void CSlider::OnUpdate() {
	POINT a = GetAbsolutePos();
	m_iHeight = 15;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x;//-1
			if (NewX < 0)
				NewX = 0;
			if (NewX > m_iWidth)
				NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}


#pragma endregion Implementations of the Slider functions

#pragma region KeyBinders

char* KeyStrings[254] = { "[ _ ]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
"[ _ ]", "[BSPC]", "[TAB]", "[ _ ]", "[ _ ]", "[ _ ]", "[ENTER]", "[ _ ]", "[ _ ]", "[SHI]",
"[CTRL]", "[ALT]","[PAU]","[CAPS]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ESC]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[SPACE]","[PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[LEFT]",
"[UP]", "[RIGHT]", "[DOWN]", "[ _ ]", "[PRNT]", "[ _ ]", "[PRTSCR]", "[INS]","[DEL]", "[ _ ]", "[0]", "[1]",
"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
"[V]", "[W]", "[X]","[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[ _ ]", "[ _ ]", "[ _ ]", "[NUM0]", "[NUM1]",
"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]","[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]","[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]", "[F21]",
"[F22]", "[F23]", "[F24]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]",
"[NUM LOCK]", "[SCROLL LOCK[", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
"[RCTRL]", "[LMENU]", "[RMENU]", "[ _ ]","[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[NTRK]", "[PTRK]", "[STOP]", "[PLAY]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[;]", "[+]", "[,]", "[-]", "[.]", "[/?]", "[~]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[{]", "[\\|]", "[}]", "['\"]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]" };

CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (this->Getting_New_Key)
	{
		Render::Text2(a.x, a.y, "[ _ ]", Render::Fonts::Menu, Color(110, 110, 110, 255));
	}
	else
	{
		if (key == -1)
			Render::Text2(a.x, a.y, "[ _ ]", Render::Fonts::Menu, Color(110, 110, 110, 255));
		else
		{
			char* NameOfKey = KeyStrings[key];
			Render::Text2(a.x, a.y, NameOfKey, Render::Fonts::Menu, Color(110, 110, 110, 255));
		}
	}

}

void CKeyBind::OnUpdate() {
	m_iHeight = 13;
	RECT text_area;
	if (key == -1)
		text_area = Render::GetTextSize(Render::Fonts::Menu, "[ _ ]");
	else
		text_area = Render::GetTextSize(Render::Fonts::Menu, text);
	m_iWidth = text_area.right;
	POINT a = GetAbsolutePos();
	if (Getting_New_Key)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Getting_New_Key = false;
					key = -1;
					text = "[ _ ]";
					return;
				}

				key = i;
				Getting_New_Key = false;
				text = KeyStrings[i];
				return;
			}
		}
	}
}

void CKeyBind::OnClick() {
	POINT a = GetAbsolutePos();
	if (!Getting_New_Key)
		Getting_New_Key = true;
}

int CKeyBind::GetKey()
{
	return key;
}

void CKeyBind::SetKey(int k)
{
	key = k;
	text = KeyStrings[k];
}

#pragma endregion Implementations of the KeyBind Control functions

#pragma region Button
CButton::CButton()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, 255), Color(55, 55, 55, 255));
	else
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, 255));

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));

	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text2(TextX, TextY, Text.c_str(), Render::Fonts::Menu, Color(180, 180, 180, 200));
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 25;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions

#pragma region ButtonSave
CButtonSave::CButtonSave()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Save";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButtonSave::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, 255), Color(55, 55, 55, 255));
	else
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, 255));

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));

	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text2(TextX, TextY, Text.c_str(), Render::Fonts::Menu, Color(180, 180, 180, 200));
}

void CButtonSave::SetText(std::string text)
{
	Text = text;
}

void CButtonSave::SetCallback(CButtonSave::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButtonSave::OnUpdate()
{
	m_iHeight = 25;
}

void CButtonSave::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions



#pragma region ButtonLoad
CButtonLoad::CButtonLoad()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Load";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButtonLoad::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, 255), Color(55, 55, 55, 255));
	else
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, 255));

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));

	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text2(TextX, TextY, Text.c_str(), Render::Fonts::Menu, Color(180, 180, 180, 200));
}

void CButtonLoad::SetText(std::string text)
{
	Text = text;
}

void CButtonLoad::SetCallback(CButtonLoad::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButtonLoad::OnUpdate()
{
	m_iHeight = 25;
}

void CButtonLoad::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions








#pragma region ButtonApplySkin
CButtonApplySkin::CButtonApplySkin()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Apply";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButtonApplySkin::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, 255), Color(55, 55, 55, 255));
	else
		Render::GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, 255));

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));

	RECT TextSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text2(TextX, TextY, Text.c_str(), Render::Fonts::Menu, Color(180, 180, 180, 200));
}

void CButtonApplySkin::SetText(std::string text)
{
	Text = text;
}

void CButtonApplySkin::SetCallback(CButtonApplySkin::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButtonApplySkin::OnUpdate()
{
	m_iHeight = 25;
}

void CButtonApplySkin::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions


#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 20;
	FileControlType = UIControlTypes::UIC_ComboBox;
}


void CComboBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	Render::GradientV(a.x, a.y, m_iWidth, 20, Color(45, 45, 45, 255), Color(45, 45, 45, 255));
	if (GUI.IsMouseInRegion(Region)) Render::GradientV(a.x, a.y, m_iWidth, 20, Color(55, 55, 55, 255), Color(55, 55, 55, 255));
	Render::Outline(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, 255));

	if (Items.size() > 0)
	{
		RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, GetItem().c_str());
		Render::Text2(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), GetItem().c_str(), Render::Fonts::Menu, Color(180, 180, 180, 245));

		if (IsOpen)
		{
			Render::GradientV(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(45, 45, 45, 255), Color(45, 45, 45, 255));

			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 20, m_iWidth, 20 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::GradientV(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(35, 35, 35, 255), Color(35, 35, 35, 255));
				}

				RECT control_textsize = Render::GetTextSize(Render::Fonts::Menu, Items[i].c_str());
				if (i == SelectedIndex)
					Render::Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), Render::Fonts::Menu, Color(0, 77, 230, 255));
				else
					Render::Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), Render::Fonts::Menu, Color(180, 180, 180, 245));
			}
			Render::Outline(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(2, 2, 2, 255));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 9;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 9;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 12;

	Render::Polygon(3, Verts2, Color(92, 92, 92, 255));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 20 + 20 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 20;
	}

}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 20 + i * 20, m_iWidth, 20 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma region TextField
char* KeyDigitsLowercase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

char* KeyDigitsCapital[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_TextField;
	m_iHeight = 12;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	std::string drawn_text = "[";

	const char *cstr = text.c_str();

	drawn_text += cstr;

	if (IsGettingKey)
		drawn_text += "_";

	drawn_text += "]";

	Render::Text2(a.x, a.y, drawn_text.c_str(), Render::Fonts::Menu, Color(244, 244, 244, 255));
}

void CTextField::OnUpdate()
{
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitsCapital[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigitsCapital[i];
					}
					else
					{
						text = text + KeyDigitsLowercase[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}
#pragma endregion Implementations of the Text Field functions

#define LIST_ITEM_HEIGHT 16
#define LIST_SCROLL_WIDTH 8

#pragma region ListBox
CListBox::CListBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	SelectedIndex = 0;
	FileControlType = UIControlTypes::UIC_ListBox;
}

void CListBox::Draw(bool hover)
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	Render::DrawRect(a.x + 1, a.y + 1, m_iWidth - 2, m_iHeight - 2, Color(90, 90, 90, 1));

	//Top Left
	Render::Line(a.x, a.y, a.x + m_iWidth - 2, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - 1, a.y - 1, Color(0, 0, 0, 255));

	//Top Right
	Render::Line(a.x + (m_iWidth / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, 255));
	Render::Line(a.x + (m_iWidth / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 255));

	//Left
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(49, 49, 49, 255));
	Render::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 255));

	//Bottom
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, 255));
	Render::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	//Right
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(48, 48, 48, 255));
	Render::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(92, 92, 92, 255);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };

			if (i == SelectedIndex)
			{
				textColor = Color(245, 245, 245, 245);



				Render::GradientV(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(0, 77, 230, 255), Color(0, 77, 230, 255));
			}
			else if (GUI.IsMouseInRegion(ItemRegion))
			{
				textColor = Color(245, 245, 245, 245);
				Render::DrawRect(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(92, 92, 92, 255));
			}

			Render::Text2(ItemRegion.left + 4, ItemRegion.top + 2, Items[i].c_str(), Render::Fonts::Menu, textColor);
			drawnItems++;
		}

		// Ratio of how many visible to how many are hidden
		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		if (sizeRatio > 1.f) sizeRatio = 1.f;
		float posRatio = float(ScrollTop) / float(Items.size());
		if (posRatio > 1.f) posRatio = 1.f;

		sizeRatio *= m_iHeight;
		posRatio *= m_iHeight;

		Render::DrawRect(a.x + m_iWidth - LIST_SCROLL_WIDTH, a.y + posRatio, LIST_SCROLL_WIDTH, sizeRatio, Color(52, 52, 52, 255));
	}

}

void CListBox::AddItem(std::string text, int value)
{
	Items.push_back(text);
	Values.push_back(value);
}

void CListBox::OnClick()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Check the items
	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(92, 92, 92, 255);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };
			if (GUI.IsMouseInRegion(ItemRegion))
			{
				SelectItem(i);
				return;
			}
			drawnItems++;
		}
	}
}

void CListBox::OnUpdate()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Did we click in the scrollbar??
	RECT Scroll = { a.x + m_iWidth - LIST_SCROLL_WIDTH , a.y + 1, LIST_SCROLL_WIDTH - 2 ,m_iHeight };

	if (GUI.IsMouseInRegion(Scroll) && GetAsyncKeyState(VK_LBUTTON)) dragging = true;
	else if (!GetAsyncKeyState(VK_LBUTTON) && dragging) dragging = false;

	if (dragging)
	{
		// Ratio of how many visible to how many are hidden
		float ratio = float(ItemsToDraw) / float(Items.size());
		POINT m = GUI.GetMouse();
		m.y -= a.y;

		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		sizeRatio *= m_iHeight;
		float heightDelta = m.y + sizeRatio - m_iHeight;
		if (heightDelta > 0)
			m.y -= heightDelta;

		float mPosRatio = float(m.y) / float(m_iHeight);
		ScrollTop = mPosRatio * Items.size();
		if (ScrollTop < 0)
			ScrollTop = 0;
	}
}

void CListBox::SetHeightInItems(int items)
{
	m_iHeight = items * LIST_ITEM_HEIGHT;
}

std::string CListBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}
#pragma endregion Implementations of the ComboBox functions
#pragma region SilentGroupBox
CSilentGroupBox::CSilentGroupBox()
{
	Items = 1;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
}

void CSilentGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, Text.c_str());
}

void CSilentGroupBox::SetText(std::string text)
{
	Text = text;
}

void CSilentGroupBox::PlaceLabledControl(std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 16;
	int y = m_y + Items * 18;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);

	x += m_iWidth / 2;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);
	Items++;
}

void CSilentGroupBox::OnUpdate() {}
void CSilentGroupBox::OnClick() {}
#pragma endregion Implementations of the Silent Group Box functions


#pragma region MultiBox
CMultiBox::CMultiBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 40;
	FileControlType = UIControlTypes::UIC_MultiBox;
}

void CMultiBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::outline(a.x, a.y, m_iWidth, (items.size() * 16) + 19, Color(2, 2, 2, 255));
	Render::GradientV(a.x + 1, a.y + 1, m_iWidth - 2, 24, Color(35, 35, 35, 255), Color(39, 39, 39, 255));

	RECT TitleSize = Render::GetTextSize(Render::Fonts::Menu, title);
	int title_x = a.x + (m_iWidth / 2) - (TitleSize.right / 2);
	int title_y = a.y + 8 - (TitleSize.bottom / 2);
	Render::text(title_x, title_y, title, Render::Fonts::Menu, Color(255, 255, 255, 255));

	Render::GradientV(a.x + 1, a.y + 18, m_iWidth - 2, items.size() * 16, Color(55, 55, 55, 255), Color(40, 40, 40, 245));

	for (int i = 0; i < items.size(); i++)
	{
		MultiBoxItem item = items[i];
		RECT txtsize = Render::GetTextSize(Render::Fonts::Menu, item.text);
		int item_x = a.x + (m_iWidth / 2) - (txtsize.right / 2);
		int item_y = a.y + 19 + (i * 16) - (txtsize.bottom / 2) + 7;
		if (!item.bselected)
			Render::text(item_x, item_y, item.text, Render::Fonts::Menu, Color(245, 245, 245, 245));
		else
			Render::text(item_x, item_y, item.text, Render::Fonts::Menu, Color(167, 244, 66, 255));
	}
}

void CMultiBox::OnUpdate()
{
	m_iHeight = (items.size() * 16) + 19;

}

void CMultiBox::OnClick()
{
	POINT a = GetAbsolutePos();
	for (int i = 0; i < items.size(); i++)
	{
		RECT area = { a.x, a.y + i * 16 + 19, m_iWidth, 16 };
		if (GUI.IsMouseInRegion(area))
		{
			items[i].bselected = !items[i].bselected;
		}
	}
}

void CMultiBox::SetTitle(const char* tl)
{
	title = tl;
}
#pragma endregion Implementations of the MultiBox functions