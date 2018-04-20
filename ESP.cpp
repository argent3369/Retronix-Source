#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "GlowManager.h"
#include "Autowall.h"
#include <stdio.h>
#include <stdlib.h>
#include "LagComp.h"
#include "Hooks.h"
#define M_PI 3.14159265358979323846
DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);
int espstuff::FakeDetection[64];
#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif

void CEsp::Init()
{
	BombCarrier = nullptr;
}

void CEsp::Move(CUserCmd *pCmd,bool &bSendPacket) 
{

}

void CEsp::Draw()
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;


	IClientEntity *pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		if (pEntity && !pEntity->IsDormant())
		{

			if (Menu::Window.VisualsTab.OtherRadar.GetState())
			{

				DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
				*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
			}



			if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{

				DrawPlayer(pEntity, pinfo);
			}

			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

			if (Menu::Window.VisualsTab.FiltersNades.GetState() && strstr(cClass->m_pNetworkName, "Projectile"))
			{

		//		DrawThrowable(pEntity);
			}

			if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
			{

				DrawDrop(pEntity, cClass);
			}

			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					DrawBombPlanted(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					BombTimer(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CC4)
					DrawBomb(pEntity, cClass);
			}
		}

	}

	if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
	{
		DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
		*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = 0;
	}

	if (Menu::Window.VisualsTab.OptionsGlow.GetState())
	{

		DrawGlow();
	}
	if (Menu::Window.VisualsTab.EntityGlow.GetState())
	{

		EntityGlow();
	}
}





void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	int x;
	int y;
	int height;
	ESPBox Box;
	Color Color;

	Vector max = pEntity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = pEntity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	if (!Render::WorldToScreen(pos3D, pos) || !Render::WorldToScreen(top3D, top))
		return;

	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;

	if (!Menu::Window.VisualsTab.FiltersSelf.GetState() && pEntity == hackManager.pLocal())
		return;

	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		switch (Menu::Window.VisualsTab.OptionsBox.GetIndex())
		{
		case 0:
			break;
		case 1:
			DrawBox(Box, Color);
			break;
		case 2:
			DrawBox69(Box, Color);
			break;
		}

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);



			if (Menu::Window.VisualsTab.OptionHealthEnable.GetState())
				DrawHealth2(pEntity, Box);
	
	


		if (Menu::Window.VisualsTab.OptionsInfo.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetState())
			DrawInfo(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsArmor.GetState())
			Armor(pEntity, Box);

		if (Menu::Window.VisualsTab.Barrels.GetState())
			Barrel(Box, Color, pEntity);

		if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
			DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);


		if (Menu::Window.LegitBotTab.AimbotBacktrack.GetState())
			BacktrackingCross(pEntity);




	}
}

void CEsp::DrawBox69(CEsp::ESPBox size, Color color)
{
	Render::Outline(size.x, size.y, size.w, size.h, color);
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(255, 255, 255, 160));
}
void CEsp::DrawHealth2(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float flBoxes = std::ceil(HealthValue / 10.f);
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;
	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;
	Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);

	UINT hp = h - (UINT)((h * health) / 100); // Percentage

	int Red = 255 - (health*2.55);
	int Green = health * 2.55;

	Render::Outline((size.x - 6), size.y - 1, 3, h + 2, Color(0, 0, 0, 255));
	Render::Outline(size.x - 5, size.y - 1, 1, (h / 10) * flBoxes + 1, ColHealth);

	for (int i = 0; i < 10; i++)
	{
		Render::Line((size.x - 6), size.y + i * (h / 10) - 1, size.x - 3, size.y + i * (h / 10), Color(0, 0, 0, 255));
	}
}

void CEsp::DoEnemyCircle(IClientEntity* pEntity, const Vector &vecDelta, float *flRotation)
{

}

void CEsp::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };


	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!Render::WorldToScreen(corners[edge[0]], p1) || !Render::WorldToScreen(corners[edge[1]], p2))
			return;
		Color(0, 255, 0, 255);
	}
}


bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

void CEsp::Corners(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.30f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));

	Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));

	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
}

Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.TColorNoVisR.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255);
	}
	else
	{

		if (IsVis)
			color = Color(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255);
	}

	return color;
}



void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{

	// Corner Box
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	// Full Box
	Render::Outline(size.x, size.y, size.w, size.h, color);
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
	Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
}

void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{

	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


	y = ScreenHeight;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(0, 255, 0, 255));
}

std::string CleanItemName(std::string name)
{

	std::string Name = name;
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

void CEsp::DrawGlow()
{

	int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 35:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;
				if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;

				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}

				else
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}
			}
		}
	}

}

void CEsp::EntityGlow()
{
	int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 1:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (Menu::Window.VisualsTab.EntityGlow.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));

			}
		case 9:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				if (Menu::Window.VisualsTab.EntityGlow.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 29:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 39:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));

			}
		case 41:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 66:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 87:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 98:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 108:
			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));

			}
		case 130:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 134:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));

			}
		default:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "Weapon"))
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		}

	}
}

static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size/4, text, size);
	return wa;
}

void CEsp::BacktrackingCross(IClientEntity* pEntity)
{
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{

		player_info_t pinfo;
		if (pEntity && pEntity != hackManager.pLocal() && !pEntity->IsDormant())
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{

				if (Menu::Window.LegitBotTab.AimbotBacktrack.GetState())
				{

					if (hackManager.pLocal()->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{

							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > hackManager.pLocal()->GetSimulationTime())
							{

								if (Render::WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									Interfaces::Surface->DrawSetColor(Color(255, 0, 0, 255));
									Interfaces::Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}

							}
						}

					}
					else
					{

						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				if (Menu::Window.RageBotTab.AccuracyBacktracking.GetState())
				{

					if (hackManager.pLocal()->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{

							Vector screenbacktrack[64];

							if (backtracking->records[i].tick_count + 12 > Interfaces::Globals->tickcount)
							{
								if (Render::WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
								{
									Interfaces::Surface->DrawSetColor(Color(255, 0, 0, 255));
									Interfaces::Surface->DrawOutlinedRect(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

								}
		
							}
						}
			
					}
					else
					{
						memset(&backtracking->records[0], 0, sizeof(backtracking->records));
					}
				}
			}

		}
	}

}

void CEsp::BombTimer(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{

		ESPBox Box;
		GetBox(pEntity, Box);
		DrawBox(Box, Color(250, 42, 42, 255));
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		float TimeRemaining2;
		bool exploded = true;
		if (TimeRemaining < 0)
		{
			!exploded;

			TimeRemaining2 = 0;
		}
		else
		{
			exploded = true;
			TimeRemaining2 = TimeRemaining;
		}
		char buffer[64];
		if (exploded)
		{
;
			sprintf_s(buffer, "Bomb: %.1f", TimeRemaining2);
		}
		else
		{

			sprintf_s(buffer, "Bomb Fatal", TimeRemaining2);
		}
		Render::Text(vScreen.x, vScreen.y, Color(255, 255, 255, 255), Render::Fonts::ESP, buffer);
	}
}

CEsp::ESPBox CEsp::GetBOXX(IClientEntity* pEntity)
{

	ESPBox result;
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return result;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return result;
}




void CEsp::IwebzHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	// --  Main Bar -- //

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(HealthBar.x, HealthBar.y));
	Verts[1].Init(Vector2D(HealthBar.x + size.w + 5, HealthBar.y));
	Verts[2].Init(Vector2D(HealthBar.x + size.w, HealthBar.y + 5));
	Verts[3].Init(Vector2D(HealthBar.x - 5, HealthBar.y + 5));

	Render::PolygonOutline(4, Verts, Color(10, 10, 10, 255), Color(255, 255, 255, 170));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 4, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 5));
	Verts2[3].Init(Vector2D(HealthBar.x - 4, HealthBar.y + 5));

	Color c = GetPlayerColor(pEntity);
	Render::Polygon(4, Verts2, c);

	Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
	Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 2, HealthBar.y + 1));
	Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
	Verts2[3].Init(Vector2D(HealthBar.x - 2, HealthBar.y + 2));

	Render::Polygon(4, Verts2, Color(255, 255, 255, 40));

}

void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{

	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);
	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	ESPBox BOX = GetBOXX(pEntity);
	int Line_Size = (BOX.y - BOX.h) / 8;
	int Line_Size2 = (BOX.y + BOX.h) / 8;
	int Health = pEntity->GetHealth();
	if (Health > 100)
		Health = 100;
	int healthG = Health * 2.55;
	int healthR = 255 - healthG;
	Interfaces::Surface->DrawSetColor(0, 0, 0, 150);
	Interfaces::Surface->DrawOutlinedRect(BOX.x - 5, BOX.y - 1, BOX.x - 1, BOX.y + BOX.h + 1);
	//Interfaces::Surface->DrawOutlinedRect(BOX.x + BOX.w + 1, BOX.y - 1, BOX.x + BOX.w + 5, BOX.y + BOX.h + 1);

	int hpBarH = Health * BOX.h / 100;

	if (Health > 0)
	{

		Interfaces::Surface->DrawSetColor(healthR, healthG, 0, 255);
		Interfaces::Surface->DrawFilledRect(BOX.x - 4, BOX.y + BOX.h - hpBarH, BOX.x - 2, BOX.y + BOX.h);
	}

	if (Health < 100)
	{

		Render::Text(size.x - 10, size.y + hp - 2, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);
	}
}




void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	if (strlen(pinfo.name) > 16)
	{

		pinfo.name[12] = 0;
		strcat(pinfo.name, "...");
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
	else
	{

		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
	//RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	//Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
}

void DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	Interfaces::Surface->DrawSetColor(col);
	Interfaces::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	Interfaces::Surface->DrawSetColor(col);
	Interfaces::Surface->DrawLine(x0, y0, x1, y1);
}

void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{

	std::vector<std::string> Info;

	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			std::string meme = CleanItemName(cClass->m_pNetworkName);
			RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, meme.c_str());
			if (Menu::Window.VisualsTab.OptionsWeapon.GetState()) {
				Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 1,
					Color(255, 255, 255, 255), Render::Fonts::ESP, meme.c_str());
			}
		}
	}

	RECT defSize = Render::GetTextSize(Render::Fonts::ESP, "");
	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity->IsDefusing())
	{
		Render::Text(size.x + size.w + 3, size.y + (0.3*(defSize.bottom + 15)),
			Color(255, 0, 0, 255), Render::Fonts::ESP, charenc("Defusing"));
	}
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;

	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("Bomb Carrier");
	}

	if (Menu::Window.VisualsTab.ResolverInfo.GetState())
	{
		RECT scrn = Render::GetViewport();
		if (resolvokek::resolvemode == 1)
			texts.push_back(Text(std::string("Force"), 0, Render::Fonts::ESP, Color(255, 0, 0, 255)));

		else if (pEntity->GetVelocity().Length2D() < 36 && pEntity->GetVelocity().Length2D() > 20)
			texts.push_back(Text(std::string("Fake Walk"), 0, Render::Fonts::ESP, Color(255, 0, 0, 255)));

		else if (resolvokek::resolvemode == 2)
			texts.push_back(Text(std::string("Moving LBY"), 0, Render::Fonts::ESP, Color(0, 255, 0, 255)));

		else if (resolvokek::resolvemode == 3)
			texts.push_back(Text(std::string("Resolving"), 0, Render::Fonts::ESP, Color(255, 150, 0, 255)));

		if (resolvokek::resolvemode == 4)
			texts.push_back(Text(std::string("Prediction"), 0, Render::Fonts::ESP, Color(255, 0, 0, 255)));


		else if (pEntity->GetLowerBodyYaw() > 35)
			texts.push_back(Text(std::string("Fake"), 0, Render::Fonts::ESP, Color(255, 0, 0, 255)));

		else if (pEntity->GetLowerBodyYaw() < 35)
			texts.push_back(Text(std::string("LBY Update"), 0, Render::Fonts::ESP, Color(0, 255, 0, 255)));

	}



	static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	int i = 0;
	for (auto Text : Info)
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, Text.c_str());
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 27, Color(255, 255, 0, 255), Render::Fonts::ESP, Text.c_str());
		//Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

void CEsp::DrawCross(IClientEntity* pEntity)
{

	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}

}

void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{

	Vector Box;
	IClientEntity* Weapon = (IClientEntity*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
		{

			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}

}

void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "%.1fs", TimeRemaining);
	float TimeRemaining2;
	bool exploded = true;
	if (TimeRemaining < 0)
	{
		!exploded;

		TimeRemaining2 = 0;
	}
	else
	{
		exploded = true;
		TimeRemaining2 = TimeRemaining;
	}
	if (exploded)
	{

		sprintf_s(buffer, "Bomb: %.1f", TimeRemaining2);
	}
	else
	{

		sprintf_s(buffer, "Bomb Undefusable", TimeRemaining2);
	}

	Render::Text(10, 45, Color(0, 255, 0, 255), Render::Fonts::Clock, buffer);

}

void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{

	BombCarrier = nullptr;
	C_BaseCombatWeapon *BombWeapon = (C_BaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{

			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(112, 230, 20, 255), Render::Fonts::ESP, "Bomb");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{

	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{

	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255,255,255,255));
			}
		}

	}
}

void CEsp::Armor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArBar = size;
	ArBar.y += (ArBar.h + 3);
	ArBar.h = 6;

	float ArValue = pEntity->ArmorValue();
	float ArPerc = ArValue / 100.f;
	float Width = (size.w * ArPerc);
	ArBar.w = Width;

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(ArBar.x, ArBar.y));
	Verts[1].Init(Vector2D(ArBar.x + size.w + 0, ArBar.y));
	Verts[2].Init(Vector2D(ArBar.x + size.w, ArBar.y + 2));
	Verts[3].Init(Vector2D(ArBar.x - 0, ArBar.y + 2));

	Render::PolygonOutline(4, Verts, Color(50, 50, 50, 255), Color(50, 50, 50, 255));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(ArBar.x, ArBar.y + 1));
	Verts2[1].Init(Vector2D(ArBar.x + ArBar.w + 0, ArBar.y + 1));
	Verts2[2].Init(Vector2D(ArBar.x + ArBar.w, ArBar.y + 2));
	Verts2[3].Init(Vector2D(ArBar.x, ArBar.y + 2));

	Color c = GetPlayerColor(pEntity);
	Render::Polygon(4, Verts2, Color(0, 120, 255, 200));
}

void CEsp::BoxAndText(IClientEntity* entity, std::string text)
{

	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(entity, Box))
	{
		Info.push_back(text);
		if (Menu::Window.VisualsTab.FiltersNades.GetState())
		{
			int i = 0;
			for (auto kek : Info)
			{
				Render::Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), Render::Fonts::ESP, kek.c_str());
				i++;
			}
		}

	}
}

void CEsp::DrawThrowable(IClientEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	Interfaces::ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE Grenade";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}
	BoxAndText(throwable, nadeName);
}

































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class itciehe {
public:
	string esjmjzunnlqogw;
	string ohitjr;
	int eyrdpfqbn;
	itciehe();
	string dzkdzaelykklvhcoqbomrskyu(int cytxa);
	void acjqlwgtaylthepzpfaop(int pdappdmvtsuxukc, int zdhmqf, double xyyrtbtfodcjv, string fejinrx, double gerue, string wgclts, double kkwjwsnzgrmrrrw, int cibddvegly);
	bool viudklazfylsym(string vgoprahgwbn);

protected:
	bool xotzljjphsa;
	bool nvmzausxk;
	bool dbaxrldctrd;
	string pqcfyohumvjmf;
	double yighprlicvv;

	string nmlrorkptcqdtrbsofri(bool itcohj, double znnhwtggil, bool fwvncyqpbbjzitj);
	void qgsfrrnwxrnbecduwdvjbhihb(int vtxagycwrezkubz, int gdjcnxqlheznkq, double wsufaccfu, string bltvdlfmab, bool ykecr, int izezkl, int ojspmzkza, string ksioanrpob, int byjyivcwge);
	double lnjhuiyvuyyu(double pmvmzvcttfuvtj, int efajdtnahdmvx, int xthfnvwq, bool nhdewycnov);

private:
	double oihjh;

	bool hejynqevngkbdibrb(double npmtjxygs);
	double bhflcdkuag(double uvkbjrdmrpbg, bool fxcqe, bool yhmhnpbwsfxjtag, bool ujdfu, bool dqbtogokgsjp);
	void gamzyqoaopeiu(bool hymfr, double iqsvsoiymr, bool jjspzjz, double gotmoylvmpdgnp, string rcrsryeknf, bool dinjrprdft);
	double lflgpytfvazyxtxppahfecnb(bool uihtjh, string hiekofxaknu, int jateidgqbz, bool uafqrlzsxdrfsz, bool asazukni, int vvdcze);
	void brezfkoppl(string gobuwzesu, bool dhiwtzaf, int qsutlxkdz, bool hvibiajso, double llzafanwqqwnw, double hifez, int pqigsjsbvhbt, string bwciyppw, bool cegwputie);
	bool vicgoczhobthmcoymsauasen(string igacfkj, bool ohhwh, double aoetstotufacu, bool uwmwhnfas, bool rfdpeocstzcrgrb, int uhkytkiurb, string zolmybyhcvuiju, int hdcekcanxkayu, int qlujwddrhaypnls);
	double tojofndexjqygsaykgqs();

};


bool itciehe::hejynqevngkbdibrb(double npmtjxygs) {
	int eoocwwaroccw = 1266;
	string vvtmcz = "qgubluulaxmnqnqlfzgbxlulxodxsjyd";
	double gpqtoktzos = 12537;
	int xcozfdfr = 441;
	int hsnrc = 789;
	if (string("qgubluulaxmnqnqlfzgbxlulxodxsjyd") != string("qgubluulaxmnqnqlfzgbxlulxodxsjyd")) {
		int rrnetipz;
		for (rrnetipz = 4; rrnetipz > 0; rrnetipz--) {
			continue;
		}
	}
	if (441 != 441) {
		int nmxdigpm;
		for (nmxdigpm = 14; nmxdigpm > 0; nmxdigpm--) {
			continue;
		}
	}
	return true;
}

double itciehe::bhflcdkuag(double uvkbjrdmrpbg, bool fxcqe, bool yhmhnpbwsfxjtag, bool ujdfu, bool dqbtogokgsjp) {
	string idgpsrg = "ruugmealswwfarabohggvnqnygvejwrkuyrhuthlgmaiyg";
	string gdmpzr = "cibutycfffmwoohzbglupgksowsensnmzawwdoxarrrefafxx";
	bool mjszl = false;
	string vegulfdnqhnhjyd = "h";
	int pahpkqv = 735;
	int zecul = 1040;
	bool sqxbxj = true;
	if (735 == 735) {
		int oxmziulpp;
		for (oxmziulpp = 74; oxmziulpp > 0; oxmziulpp--) {
			continue;
		}
	}
	return 46965;
}

void itciehe::gamzyqoaopeiu(bool hymfr, double iqsvsoiymr, bool jjspzjz, double gotmoylvmpdgnp, string rcrsryeknf, bool dinjrprdft) {
	int wrzqhloenpzs = 5393;
	if (5393 == 5393) {
		int iuzpchhrj;
		for (iuzpchhrj = 11; iuzpchhrj > 0; iuzpchhrj--) {
			continue;
		}
	}
	if (5393 != 5393) {
		int foektxl;
		for (foektxl = 41; foektxl > 0; foektxl--) {
			continue;
		}
	}
	if (5393 == 5393) {
		int pmofdf;
		for (pmofdf = 73; pmofdf > 0; pmofdf--) {
			continue;
		}
	}
	if (5393 == 5393) {
		int faz;
		for (faz = 58; faz > 0; faz--) {
			continue;
		}
	}

}

double itciehe::lflgpytfvazyxtxppahfecnb(bool uihtjh, string hiekofxaknu, int jateidgqbz, bool uafqrlzsxdrfsz, bool asazukni, int vvdcze) {
	int swdqfv = 3467;
	return 99425;
}

void itciehe::brezfkoppl(string gobuwzesu, bool dhiwtzaf, int qsutlxkdz, bool hvibiajso, double llzafanwqqwnw, double hifez, int pqigsjsbvhbt, string bwciyppw, bool cegwputie) {
	int izzxuqbktadqu = 2037;
	string vgotmmstqns = "fnquywdtrfkivtpzsuhsyrzuohtqlcdbxgwzmgjtelpflyjzbykvjloubevbd";
	bool gtpps = false;
	double wtywqaxe = 1037;
	double ymyld = 43623;
	bool pdxcfoycl = false;
	double vhienk = 5876;
	bool gpcjycfrwfcinmd = true;
	string efkidqn = "juhuggzvghapmjoipocerzbvfjqeudcdnrwiaitocobpregkgzolhdafpbequgeqmztuumhmrtfskdsyfkdmj";
	if (true != true) {
		int jpuoeuragt;
		for (jpuoeuragt = 13; jpuoeuragt > 0; jpuoeuragt--) {
			continue;
		}
	}
	if (1037 != 1037) {
		int my;
		for (my = 96; my > 0; my--) {
			continue;
		}
	}
	if (2037 == 2037) {
		int ddjcqfpt;
		for (ddjcqfpt = 23; ddjcqfpt > 0; ddjcqfpt--) {
			continue;
		}
	}
	if (43623 != 43623) {
		int yvwdgscnje;
		for (yvwdgscnje = 32; yvwdgscnje > 0; yvwdgscnje--) {
			continue;
		}
	}
	if (1037 != 1037) {
		int gkvgjavlce;
		for (gkvgjavlce = 53; gkvgjavlce > 0; gkvgjavlce--) {
			continue;
		}
	}

}

bool itciehe::vicgoczhobthmcoymsauasen(string igacfkj, bool ohhwh, double aoetstotufacu, bool uwmwhnfas, bool rfdpeocstzcrgrb, int uhkytkiurb, string zolmybyhcvuiju, int hdcekcanxkayu, int qlujwddrhaypnls) {
	return true;
}

double itciehe::tojofndexjqygsaykgqs() {
	double gcrdmngbiwc = 11870;
	double niuuahwbbdyetk = 42150;
	int xzerhsow = 3892;
	bool lsvtqk = false;
	string nzkkdmyrxnp = "hpnbbjpmxzadmaadizsptflvxmiyrtoybjuklbxhwkbaddnzpblhnyusumzwvsfwnnl";
	bool zpliw = true;
	bool qwqxzpl = true;
	bool idxinc = false;
	if (11870 != 11870) {
		int dkfqdfzei;
		for (dkfqdfzei = 3; dkfqdfzei > 0; dkfqdfzei--) {
			continue;
		}
	}
	if (11870 != 11870) {
		int hpiazz;
		for (hpiazz = 90; hpiazz > 0; hpiazz--) {
			continue;
		}
	}
	if (true == true) {
		int hyabid;
		for (hyabid = 21; hyabid > 0; hyabid--) {
			continue;
		}
	}
	if (false == false) {
		int aocethjc;
		for (aocethjc = 82; aocethjc > 0; aocethjc--) {
			continue;
		}
	}
	if (false != false) {
		int bkjnogf;
		for (bkjnogf = 34; bkjnogf > 0; bkjnogf--) {
			continue;
		}
	}
	return 42747;
}

string itciehe::nmlrorkptcqdtrbsofri(bool itcohj, double znnhwtggil, bool fwvncyqpbbjzitj) {
	string pyifbcacejgucsq = "mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow";
	if (string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow") != string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow")) {
		int mfc;
		for (mfc = 32; mfc > 0; mfc--) {
			continue;
		}
	}
	if (string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow") != string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow")) {
		int lihyiodv;
		for (lihyiodv = 47; lihyiodv > 0; lihyiodv--) {
			continue;
		}
	}
	if (string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow") != string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow")) {
		int ra;
		for (ra = 20; ra > 0; ra--) {
			continue;
		}
	}
	if (string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow") != string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow")) {
		int dehhc;
		for (dehhc = 47; dehhc > 0; dehhc--) {
			continue;
		}
	}
	if (string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow") != string("mpakxxsxyhwzwasyqbzynhjuxeqvxnhfsyzwdtugvgjunhkacxkpktinhjrnloisokwzxcvpwgvullbufwtfkytgcokxvuow")) {
		int ybcpnzubcc;
		for (ybcpnzubcc = 90; ybcpnzubcc > 0; ybcpnzubcc--) {
			continue;
		}
	}
	return string("flijibtmjwwzyhqhxie");
}

void itciehe::qgsfrrnwxrnbecduwdvjbhihb(int vtxagycwrezkubz, int gdjcnxqlheznkq, double wsufaccfu, string bltvdlfmab, bool ykecr, int izezkl, int ojspmzkza, string ksioanrpob, int byjyivcwge) {
	string gdlslkyaxft = "ybdkhtubrurrteafeehnyeyjmlrbwanfhwcwd";
	bool rdyfkxna = false;
	bool nvabxulubitc = true;
	int ekenfbezpk = 3692;
	int tmqsfuidvkksdua = 511;
	string nvdfcenkno = "ywpnndjlktzlnkwxmjyelhtjylxlrrhzirlzqlhefyqaglrajkynyvydcxokuhrx";
	bool xahseqku = false;
	bool vsafcsx = false;
	string jxsvdhwqsdbrlt = "kxpachnrxcapcpubrszzlxeiwhheuatroshdmklkjxpvrhouwvvzmyxq";
	int uyvalccvcp = 3441;
	if (3692 != 3692) {
		int yonriyeiwc;
		for (yonriyeiwc = 65; yonriyeiwc > 0; yonriyeiwc--) {
			continue;
		}
	}

}

double itciehe::lnjhuiyvuyyu(double pmvmzvcttfuvtj, int efajdtnahdmvx, int xthfnvwq, bool nhdewycnov) {
	int dntciuj = 1129;
	bool zsfghwlolx = true;
	string ninzntceufjqxw = "bwzfwjgjcydonjndsuxyisqkmuviphuuckwpnqqdbrlvubcmqwbpzkfrrrnyslkanvrdutfeudbpfntqbys";
	bool drhyvsqkyxnolu = true;
	int jkarfsobes = 1096;
	int qcqqmdayxpwu = 1724;
	string fbumdjv = "pv";
	bool jxzeovbng = false;
	string gjlekphiwc = "uszelhkwvggkuwjbugugitpaumhqtxvrinceoyogrdbknysxadnzjmmbbynxhjdvnbkikvop";
	double kkeillkyk = 5860;
	if (1096 != 1096) {
		int csnqs;
		for (csnqs = 90; csnqs > 0; csnqs--) {
			continue;
		}
	}
	if (string("pv") != string("pv")) {
		int rkiej;
		for (rkiej = 32; rkiej > 0; rkiej--) {
			continue;
		}
	}
	if (string("bwzfwjgjcydonjndsuxyisqkmuviphuuckwpnqqdbrlvubcmqwbpzkfrrrnyslkanvrdutfeudbpfntqbys") != string("bwzfwjgjcydonjndsuxyisqkmuviphuuckwpnqqdbrlvubcmqwbpzkfrrrnyslkanvrdutfeudbpfntqbys")) {
		int nzrxvjit;
		for (nzrxvjit = 5; nzrxvjit > 0; nzrxvjit--) {
			continue;
		}
	}
	if (1129 != 1129) {
		int tfou;
		for (tfou = 17; tfou > 0; tfou--) {
			continue;
		}
	}
	if (1724 == 1724) {
		int slcpx;
		for (slcpx = 58; slcpx > 0; slcpx--) {
			continue;
		}
	}
	return 13079;
}

string itciehe::dzkdzaelykklvhcoqbomrskyu(int cytxa) {
	int fhbha = 3908;
	bool ftumzllnljie = true;
	if (3908 != 3908) {
		int ewbpfr;
		for (ewbpfr = 61; ewbpfr > 0; ewbpfr--) {
			continue;
		}
	}
	if (3908 != 3908) {
		int tmooxjqa;
		for (tmooxjqa = 100; tmooxjqa > 0; tmooxjqa--) {
			continue;
		}
	}
	if (3908 != 3908) {
		int artslapjw;
		for (artslapjw = 20; artslapjw > 0; artslapjw--) {
			continue;
		}
	}
	return string("ccorrperdrr");
}

void itciehe::acjqlwgtaylthepzpfaop(int pdappdmvtsuxukc, int zdhmqf, double xyyrtbtfodcjv, string fejinrx, double gerue, string wgclts, double kkwjwsnzgrmrrrw, int cibddvegly) {
	int qrqbfsxzizuuj = 878;
	if (878 == 878) {
		int zndhqcnb;
		for (zndhqcnb = 61; zndhqcnb > 0; zndhqcnb--) {
			continue;
		}
	}

}

bool itciehe::viudklazfylsym(string vgoprahgwbn) {
	int tsyflcmngmewq = 4360;
	double tizpmdxpkprfvhq = 22507;
	double vyzulvgi = 7467;
	int tkslmiqsygunvac = 2255;
	string xxyvhhoxjqu = "mkdrasmnlinhnmvnezeatotxwxzdtkbqbblsxdlfkifdbcntdeitmmnd";
	bool lacss = true;
	string xklfgesprwkm = "qbwbdhxveimzfyulvrvizxbtkpnsqcmquiujyosdgihq";
	if (4360 != 4360) {
		int mm;
		for (mm = 3; mm > 0; mm--) {
			continue;
		}
	}
	if (2255 != 2255) {
		int fxmbuyz;
		for (fxmbuyz = 12; fxmbuyz > 0; fxmbuyz--) {
			continue;
		}
	}
	return false;
}

itciehe::itciehe() {
	this->dzkdzaelykklvhcoqbomrskyu(394);
	this->acjqlwgtaylthepzpfaop(4215, 8294, 40419, string("xydhapdgihllrxlcelwzbfdyjipmqvdoshlvyuzgnbgmlvfkaaobivtskmjdlfudozjlccaotkvlqhmymya"), 4312, string("origkusgfbzxhuvlpmxuyrnxbefelppzxayxpbdclbkyqwjkxaowojcqvwpeccvwoxapkbvwuolkzjconacpjhc"), 43361, 1609);
	this->viudklazfylsym(string("mytyoukycshglkxsqhlemlalcrbc"));
	this->nmlrorkptcqdtrbsofri(true, 9329, false);
	this->qgsfrrnwxrnbecduwdvjbhihb(2856, 3383, 3624, string("mqpscnvxmkwnslfaivsouxbxqxunqapgxznszheietezseuzhpcrenfbcubrqseunzwhknpstnlwvezgsfghxdgynrwbgyfczhhh"), false, 971, 237, string("abqgdcpxeohvthwqpbdy"), 583);
	this->lnjhuiyvuyyu(16347, 5178, 4388, false);
	this->hejynqevngkbdibrb(17762);
	this->bhflcdkuag(63922, false, true, false, false);
	this->gamzyqoaopeiu(false, 10866, true, 3764, string("zunhkkpwmieowkqwkysxprsgreubgvettwgngcpza"), false);
	this->lflgpytfvazyxtxppahfecnb(true, string("gytkaeebdwsw"), 2607, true, false, 3028);
	this->brezfkoppl(string("pupqdipql"), false, 24, true, 8016, 18206, 2229, string("ijhahfqobdfvpmflultzvvtxzxtwwdiieyeiivunirhbbtefvvtejlismdyfvyclyzkozzllfawjoxjmlorbfam"), false);
	this->vicgoczhobthmcoymsauasen(string("pqxhsbbkezpytqogytvfdpfizu"), false, 52326, true, true, 1719, string("cpttcgszfbpxhssdvqulajlsexyonhcpouporpgkaxyn"), 8017, 808);
	this->tojofndexjqygsaykgqs();
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rkgobrn {
public:
	double ffswcnw;
	rkgobrn();
	int cgifjcaaoyx(string bebzsdacwsef, bool hujlmvgvc, bool oykmvsnywtbsn, string sgznoaezqm, int smlgcb, double xbxwnqqgac, string meakxoxpg, bool utvkuiomhyppap, int atjqrcsjxmlbm);
	int jpsbiiuhalizdkfqldpruinbf(bool xjhutw, bool khvqxrxllrhzaqq, int gpjzhbryhuz, double qhwdhhaaspceasy, double zmfjzh, double ysngsmfd, int kooowojtti);
	int kkzxlihqggblzkwgbz(int bnkiqeq);
	string kotpvjvxnkgxpny(string bsmitrvqnd, int nxkvalqu, string dqynfiglwpepxi, int rggqnqwbvqyb, string mfsdunipzl, int aqxgxdzjplnzkt, bool terloiixekauq);
	int stbwyooezwkitrhpyi(double gbzcqsckr, double latpnwsxlm, double xddswibym, bool vhffkcgc, bool lxbfecsvbbuzemv);
	bool lplrxsekohxeilmk(double tszxyqezjiuc, double oyrfqegffcc);
	string pbsezguyojlqsnlhprydk(bool evndlptqxtsz);
	bool ygyezxehmagsvoorhxsol(bool ozdmdvrcdkbsrro, double wrbdrwacw, double gzulc, string rnwxsonfvz, string wdrjnlpd, string hdhnnahwtm, double tqunajjfvcv, string ceqpwcbnedmoycu, int khbsjwsveuissgl, string lmmzrghsrs);
	string szhcknzqkkwgvsrbrkizc(bool rmqszwbitdjnx, int wnbvdwxblirm, string icubcdavuvsqf, double eklwf, double myufdl, bool nbygox, bool yymnzuujtvr, int gqlokodcmyjhf);
	void mqiifyyxrsxprbdwnukx(bool wxoxfhmqixsolbm, string ucaijryyacfzeoj, int esmukhsmnlrhi, bool ncfcklaxwzt, int fysoxzts, string bhruvpncosv, double hnsks, int hszbiwhlzmcotkf);

protected:
	bool rhuvgoeuvmiumjb;
	string kxwerrty;
	string ksifn;
	double zieqejypiiqtaq;

	bool ocoltueojgvmjtjjxekaettn(string llnvfmcfr, string vlmauyqrksdq, bool grxdgov, int rbamgmdzrkiqtv, int ijtlipca, string cneqfjgdfywqto, int ojfefjlxvhvdums, bool gwchunpviumowfn, double favcietlykgpy);
	bool jknmjtreuedlx();
	void vxogezkcazj(int cnfqlvphieqsl);
	double cktyvbvzlyvalqhenmyrwe(string oaizzcuhdxjnb, bool bwllvtqyut, bool bnhrjmagd, string jzthqwwbxrfk, int wichgunnxb, bool xbbjzhjgozfibm, double opflovamryunrlz, bool bklyopuicg, bool qxyzxuvfqsbf);

private:
	bool dpbxbncpszjhnsy;
	bool litqozqxehzr;

	int uiivvkwpxw(bool qjkqx, string kvdmjliuiw, int gvxbiward);
	double hjudlkmjvhdwjbrpyro();
	bool qnmserodamcnbtlzkigwf(double nngscpnxjgwleg, int gszuckmv, int ecznkpppeq);
	double lmyawygbdtypaaedftqm(int byzog);

};


int rkgobrn::uiivvkwpxw(bool qjkqx, string kvdmjliuiw, int gvxbiward) {
	bool jpwtlcncg = true;
	bool scvgyfmai = true;
	double szlkzhibitms = 76490;
	string ugsqmcu = "ltjvebenwxbvxngdzytfgxwkvffuwvriuecsnhzfujwuvxtaeilaguqbsqilnjhpahgfgjuasioobwjmkxndhfxkbykico";
	double sryvqstbyzsprau = 4305;
	string llzfeexmt = "oxgszxvidwnmxkjndztswkisaqgusaoppseifzz";
	int rxsagszuvdbnaf = 1302;
	return 72277;
}

double rkgobrn::hjudlkmjvhdwjbrpyro() {
	return 56567;
}

bool rkgobrn::qnmserodamcnbtlzkigwf(double nngscpnxjgwleg, int gszuckmv, int ecznkpppeq) {
	double zdulwlcyfvhruib = 5198;
	bool ptlme = false;
	if (false != false) {
		int ssukqgdlpg;
		for (ssukqgdlpg = 54; ssukqgdlpg > 0; ssukqgdlpg--) {
			continue;
		}
	}
	if (5198 != 5198) {
		int zovhb;
		for (zovhb = 84; zovhb > 0; zovhb--) {
			continue;
		}
	}
	if (5198 == 5198) {
		int ynd;
		for (ynd = 51; ynd > 0; ynd--) {
			continue;
		}
	}
	if (5198 != 5198) {
		int uryzxmeht;
		for (uryzxmeht = 58; uryzxmeht > 0; uryzxmeht--) {
			continue;
		}
	}
	return false;
}

double rkgobrn::lmyawygbdtypaaedftqm(int byzog) {
	double nvlhdfnvdxz = 80762;
	double aabkrgmuj = 66874;
	int ajtafhji = 4854;
	bool bclwhr = false;
	return 15283;
}

bool rkgobrn::ocoltueojgvmjtjjxekaettn(string llnvfmcfr, string vlmauyqrksdq, bool grxdgov, int rbamgmdzrkiqtv, int ijtlipca, string cneqfjgdfywqto, int ojfefjlxvhvdums, bool gwchunpviumowfn, double favcietlykgpy) {
	string dayfoxzmxqsymn = "gibzbardibvreimfywkzkgsgddfcmdmkmlnqrkwfbskegbbucwyipdq";
	double moqkxrdlxjb = 65159;
	return true;
}

bool rkgobrn::jknmjtreuedlx() {
	string kdmysqygylagzj = "haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr";
	if (string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr") == string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr")) {
		int hrz;
		for (hrz = 47; hrz > 0; hrz--) {
			continue;
		}
	}
	if (string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr") != string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr")) {
		int fvmsdr;
		for (fvmsdr = 21; fvmsdr > 0; fvmsdr--) {
			continue;
		}
	}
	if (string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr") == string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr")) {
		int fyfrkoe;
		for (fyfrkoe = 83; fyfrkoe > 0; fyfrkoe--) {
			continue;
		}
	}
	if (string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr") != string("haknveuoypsqzgcmphxidsubdyhutlfxgcfrptixpkgmjmjfujajkgqulwbhgrsbr")) {
		int rtl;
		for (rtl = 62; rtl > 0; rtl--) {
			continue;
		}
	}
	return true;
}

void rkgobrn::vxogezkcazj(int cnfqlvphieqsl) {
	int vshofni = 2665;
	bool aclsnocnljyyvoz = true;
	double emsxyemqt = 1045;
	if (1045 != 1045) {
		int skp;
		for (skp = 54; skp > 0; skp--) {
			continue;
		}
	}
	if (true != true) {
		int xri;
		for (xri = 18; xri > 0; xri--) {
			continue;
		}
	}
	if (true == true) {
		int irsjgwpb;
		for (irsjgwpb = 9; irsjgwpb > 0; irsjgwpb--) {
			continue;
		}
	}

}

double rkgobrn::cktyvbvzlyvalqhenmyrwe(string oaizzcuhdxjnb, bool bwllvtqyut, bool bnhrjmagd, string jzthqwwbxrfk, int wichgunnxb, bool xbbjzhjgozfibm, double opflovamryunrlz, bool bklyopuicg, bool qxyzxuvfqsbf) {
	string zyljwboaltokjwm = "qpirwdisjsjlhyyiaxauuhhkbdbfhtmacxigpqbuuzakajsrr";
	int pusndzjyzg = 5269;
	bool cnzuj = true;
	bool bntqzambfwcajvf = false;
	bool zgylhqxpqlbn = false;
	int lixuaaw = 562;
	if (false == false) {
		int jx;
		for (jx = 65; jx > 0; jx--) {
			continue;
		}
	}
	if (false == false) {
		int ncygco;
		for (ncygco = 89; ncygco > 0; ncygco--) {
			continue;
		}
	}
	return 79535;
}

int rkgobrn::cgifjcaaoyx(string bebzsdacwsef, bool hujlmvgvc, bool oykmvsnywtbsn, string sgznoaezqm, int smlgcb, double xbxwnqqgac, string meakxoxpg, bool utvkuiomhyppap, int atjqrcsjxmlbm) {
	double mbkewyisu = 9328;
	return 52968;
}

int rkgobrn::jpsbiiuhalizdkfqldpruinbf(bool xjhutw, bool khvqxrxllrhzaqq, int gpjzhbryhuz, double qhwdhhaaspceasy, double zmfjzh, double ysngsmfd, int kooowojtti) {
	return 63688;
}

int rkgobrn::kkzxlihqggblzkwgbz(int bnkiqeq) {
	string pbhbgpdkxuhlzwj = "lvkisjzdoekmhbshdc";
	bool gtamtlbsbawow = false;
	string qpjwjibjgr = "vtvablbpwexybospic";
	double nruktgab = 12076;
	if (string("lvkisjzdoekmhbshdc") == string("lvkisjzdoekmhbshdc")) {
		int qjggdaelra;
		for (qjggdaelra = 19; qjggdaelra > 0; qjggdaelra--) {
			continue;
		}
	}
	if (12076 == 12076) {
		int nevip;
		for (nevip = 2; nevip > 0; nevip--) {
			continue;
		}
	}
	if (string("lvkisjzdoekmhbshdc") == string("lvkisjzdoekmhbshdc")) {
		int bdpcowug;
		for (bdpcowug = 10; bdpcowug > 0; bdpcowug--) {
			continue;
		}
	}
	return 40206;
}

string rkgobrn::kotpvjvxnkgxpny(string bsmitrvqnd, int nxkvalqu, string dqynfiglwpepxi, int rggqnqwbvqyb, string mfsdunipzl, int aqxgxdzjplnzkt, bool terloiixekauq) {
	int tggxawyke = 5950;
	bool bwcdqkt = true;
	string mxlszckwvv = "cxnbuvtxhwvibjulqrdslkzjbrtavyzscsxqziwpsfehcdekdvfwwfcoovsfmhgyjuqgjowlljdukljkivlzvelpbfkq";
	int rjgeywvtnc = 556;
	double dvxdhfddzfky = 76275;
	if (76275 == 76275) {
		int sahozpgxax;
		for (sahozpgxax = 17; sahozpgxax > 0; sahozpgxax--) {
			continue;
		}
	}
	return string("gqecja");
}

int rkgobrn::stbwyooezwkitrhpyi(double gbzcqsckr, double latpnwsxlm, double xddswibym, bool vhffkcgc, bool lxbfecsvbbuzemv) {
	return 3488;
}

bool rkgobrn::lplrxsekohxeilmk(double tszxyqezjiuc, double oyrfqegffcc) {
	return true;
}

string rkgobrn::pbsezguyojlqsnlhprydk(bool evndlptqxtsz) {
	double kiydsi = 77532;
	string zkqwgxcp = "mscgybytolaqoyhugmfemyzghandqbtxizdoknqgjydjkoyrieijsihsbngaoxlzgtdsvhgxggz";
	int trbiyduoqr = 598;
	int jpfzvvqmwegw = 858;
	double teoudkq = 25679;
	string lnvqcll = "oiznj";
	string sbiitxpic = "jlazkslgnfzyofmhpjibqblloxmzpzndgliplknlvmaxvowmsahlkwqtylfmmwhcimlblfzrgdidtqc";
	string xlxkqyshtrgqc = "nqyrhwkf";
	double phxwvwoisfkecle = 74950;
	return string("xaucdrnvhdhulbl");
}

bool rkgobrn::ygyezxehmagsvoorhxsol(bool ozdmdvrcdkbsrro, double wrbdrwacw, double gzulc, string rnwxsonfvz, string wdrjnlpd, string hdhnnahwtm, double tqunajjfvcv, string ceqpwcbnedmoycu, int khbsjwsveuissgl, string lmmzrghsrs) {
	double xwfgmmblccizox = 77873;
	string kskzyhyre = "ochzunfuscfeuczlafzlnthmxrkspllinfdzpdsyjr";
	double umvoi = 25312;
	double cmyjtwn = 41841;
	bool tnjadjamkdz = true;
	string vdctwiwptyw = "vloorgxzvwehencgqpjnaifgclxpyrfuunkvzzigfticzlslebmtzhahlgjgu";
	int bsxrtcqu = 1761;
	if (true != true) {
		int yetxxuwzta;
		for (yetxxuwzta = 72; yetxxuwzta > 0; yetxxuwzta--) {
			continue;
		}
	}
	if (1761 == 1761) {
		int ukvpmly;
		for (ukvpmly = 16; ukvpmly > 0; ukvpmly--) {
			continue;
		}
	}
	return false;
}

string rkgobrn::szhcknzqkkwgvsrbrkizc(bool rmqszwbitdjnx, int wnbvdwxblirm, string icubcdavuvsqf, double eklwf, double myufdl, bool nbygox, bool yymnzuujtvr, int gqlokodcmyjhf) {
	double chlwbrs = 48256;
	int bulzzsus = 1774;
	string nviusbpezvqtdu = "pxqezhwapedqpsxqpowgsnydyxglpsrscvvuqkipxfhntrkxbonvufqpdtgwanowwgiahvl";
	double ispujqevxbdszsi = 46690;
	int dcgrbv = 791;
	int nkqgsz = 9078;
	string opfboos = "gjrzhm";
	string zpcwwxhp = "epi";
	if (48256 != 48256) {
		int zbfsbxhde;
		for (zbfsbxhde = 10; zbfsbxhde > 0; zbfsbxhde--) {
			continue;
		}
	}
	if (9078 == 9078) {
		int wifq;
		for (wifq = 86; wifq > 0; wifq--) {
			continue;
		}
	}
	return string("vykjxdarccaqepnmdqi");
}

void rkgobrn::mqiifyyxrsxprbdwnukx(bool wxoxfhmqixsolbm, string ucaijryyacfzeoj, int esmukhsmnlrhi, bool ncfcklaxwzt, int fysoxzts, string bhruvpncosv, double hnsks, int hszbiwhlzmcotkf) {
	string hcjxzfyyjbmzxz = "trbszlaklspkmqukqfepzzmtrtkpiavxthqldcwqktlqqjdyeawxjwrpkjn";
	bool ghedf = false;
	double rutyqqqkrxtm = 20967;
	string ikxnitdqlawtsnj = "tkwpocqwphupqsuykcieusiklnupjpvdys";
	double snmlxuslyqfhbj = 590;
	int lwiamgaonkrgneo = 301;
	string tkyktwky = "zgmuuuvfrtnkfoupetvaxsbolsfcxonluigogtwpvlat";
	string wiuebektjbnzet = "latzeibqeoxzeses";
	string zjqufrvtncwl = "wdfqropescxdjbhhacoeedryoighbjrcwydfoykdzavgljlcszyppjzhgb";
	if (string("trbszlaklspkmqukqfepzzmtrtkpiavxthqldcwqktlqqjdyeawxjwrpkjn") == string("trbszlaklspkmqukqfepzzmtrtkpiavxthqldcwqktlqqjdyeawxjwrpkjn")) {
		int dgmuyaqqu;
		for (dgmuyaqqu = 83; dgmuyaqqu > 0; dgmuyaqqu--) {
			continue;
		}
	}
	if (string("wdfqropescxdjbhhacoeedryoighbjrcwydfoykdzavgljlcszyppjzhgb") == string("wdfqropescxdjbhhacoeedryoighbjrcwydfoykdzavgljlcszyppjzhgb")) {
		int mb;
		for (mb = 16; mb > 0; mb--) {
			continue;
		}
	}
	if (string("trbszlaklspkmqukqfepzzmtrtkpiavxthqldcwqktlqqjdyeawxjwrpkjn") != string("trbszlaklspkmqukqfepzzmtrtkpiavxthqldcwqktlqqjdyeawxjwrpkjn")) {
		int zd;
		for (zd = 0; zd > 0; zd--) {
			continue;
		}
	}
	if (false == false) {
		int ivfjqaiav;
		for (ivfjqaiav = 17; ivfjqaiav > 0; ivfjqaiav--) {
			continue;
		}
	}
	if (590 == 590) {
		int fybgvj;
		for (fybgvj = 83; fybgvj > 0; fybgvj--) {
			continue;
		}
	}

}

rkgobrn::rkgobrn() {
	this->cgifjcaaoyx(string("giazwjsiku"), true, true, string("w"), 5065, 12854, string("gmsgkrzfmkgriyqorppnetjnghdaddpbo"), false, 4160);
	this->jpsbiiuhalizdkfqldpruinbf(true, true, 3204, 9864, 37962, 60421, 1567);
	this->kkzxlihqggblzkwgbz(902);
	this->kotpvjvxnkgxpny(string("kapf"), 1745, string("pzbcqtxbmabawynkkebhpjrfribtpzqsekkzrmadyzdcigdmvspwdmboowohgzgzmhsmilvoohbyvkemutazgkaawgczio"), 3268, string(""), 351, true);
	this->stbwyooezwkitrhpyi(4335, 11587, 16741, true, false);
	this->lplrxsekohxeilmk(3363, 3702);
	this->pbsezguyojlqsnlhprydk(false);
	this->ygyezxehmagsvoorhxsol(false, 3627, 40087, string("sevsbhhppluzywhcsu"), string("csaykpkziilsinqwvwgirouarbgscjlwryvrtfcuqpbyqdwbghdwejivdkbznkjkftwpfaeyrreygflmvemexigmxzrjpzesn"), string("nwwlrghpfdyprlmzwdwmnewgghijvaxvcspzkrpttqmrfhjpbqolurldkjloarxwwv"), 32102, string("thhjybebvoahlpabwundufilzburhbkkozyshzlxfhszlbfpfuttyxsytuxflrtcynflkzwswqpgprf"), 665, string("xanxmxqwdknsmmkdasblldfncidbejedbgaruyalxaeeigcsorfcxysrxfziyimpezczarjsvzartkzfrhhvxwpysxavjhccjiam"));
	this->szhcknzqkkwgvsrbrkizc(true, 1424, string("deonwiorsaxlgccfyvnfxdssrajdcbtfulnztkyzbltlsprjwgpnnfgwfioje"), 5976, 9129, true, true, 6110);
	this->mqiifyyxrsxprbdwnukx(true, string("mafgdavgkquaqiuehtgujwzctbefaligphjmduexlumdqgqmgoamilvzallpch"), 2157, true, 2331, string("gmmcjzkuqkttipcfpji"), 52333, 7890);
	this->ocoltueojgvmjtjjxekaettn(string("ypoiasxdhsofzvcbqkxfiuoisxdouimvfuinucwpcyeebscjzpfsrzyc"), string("uytwtoejyvaiejmjxoslfyrewfzlrmeudcpibstnmawyzckjedrruojyiasajpbnuyjpckdfxhsgwy"), false, 3650, 2520, string("oskjbnuthgzfvsutelckufmawiptoazdguysifmonchihmpjhuvolwueowtlyvoxnyymfhtkcswpnwovamoqwints"), 3964, true, 48752);
	this->jknmjtreuedlx();
	this->vxogezkcazj(5291);
	this->cktyvbvzlyvalqhenmyrwe(string("mmehfbnoonnblxioheirpbijvpjkcqgflstceiulcztkdtylnpeysfsvyjipdqssxbiewhijczknlqqagkaewvbovr"), true, false, string("wb"), 2456, true, 4032, false, true);
	this->uiivvkwpxw(true, string("qmdpcunazydrjabjvdfrmaaiknio"), 2410);
	this->hjudlkmjvhdwjbrpyro();
	this->qnmserodamcnbtlzkigwf(3689, 3432, 4112);
	this->lmyawygbdtypaaedftqm(458);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jtcjslm {
public:
	double xociamnsgj;
	string esrfkkzeku;
	int wkuppmetwym;
	jtcjslm();
	double ukdggqrgatrskmfozihr(bool qlusoxxjvclbmkv, double xopeigcjyzddxco, int sheti, string rtihl, bool ddcjptxmabccca, string uzoqkilnifxmiq, double njnqalojjbzsl, bool pfeiigalwmiupim, double rearz, int hxhwzicpsrs);
	bool wmbeufefwmhylcmetmthhfpxk(string eshxbvbkeleklc, int bfcjanweiydacwf, bool hesoqdvvhf, string yxrdpikkmokyq, string nsgyfkcmkvaq, int bkststieps, bool ztyqpbuglvz, string zarksrdtnp, string repiotlcrjfk);
	int xvonexxovhqg(int iyynra, double athfqrtmjc, string fczpbwhriwpaqnu, double lguszyfcd, string epdtgvlxrnly, int nsljoeia);
	void scfdzyflcscdmg(string vcnjjwzjerwely, double fuumyvvwtwvpndp, int fyfiwxg, bool bepljkeqechdyf, int xjgiyndk, double saqsei, double lngadfygtymr);
	bool aeioqrtjbhfsgxiooftcmnr(double xioiyajgenba, double ycmxt);
	double pizcywftlxwtn();
	bool flbtoeuvgldostgw(string ntqnkieqgu);
	void gqtaimnyexgfxsipu(int xdtdvwkk, string fqqmkldkgh, bool wajuvdlkakwxb, double voegw, int lsqgoncowsvjey, string jyqhvxsd, string ymviotbs, bool ysnuseehpjladsj, bool urbopmsgyzzt);
	int uiwcitdjjofiixbpxijwlakit(string ghzhxolnkdmb, bool ouczjybeopd, string rshfovxtlnv, bool iiergeduodcg, string wfflfhzwwj, bool yviurkk, int evayugiookk, int mwbkgrfevawfldg);
	double tgxjsptdscmortk();

protected:
	string ubrol;

	int aqsmmihtxamotqp(bool inplvzxk, string bvxxkrtvxsuhz, double yvlxitiuj, double mmuhhawqpmmrz, string rbibbx);
	bool mpulbedcehcsut(double pptkcyapfd, bool xuasu, string fzvrfgxv, bool wgxgdmvhhfhigv, int naqhvke, double hyxorjdfwff);
	void lbuxwrmmyjq(string fvyyilocsfnaqau, double xemhcrjtjsey, string ldofynzwbqji, string ylugeaxctpwzbl, string kheip, string xrtwbi, string imogsa);

private:
	int dappomcqgygkz;
	double dwafjlrz;
	bool cxcvtlrkdfrn;
	bool klpdtdauwmmayb;
	double jrwgummmfvavdt;

	string hkmeegzjllc(bool fnuenby, double jqotldh, double eihrfhxctgcbmh, double entxfwrjphbfwx);

};


string jtcjslm::hkmeegzjllc(bool fnuenby, double jqotldh, double eihrfhxctgcbmh, double entxfwrjphbfwx) {
	string xvedruubkqmh = "mzczuybfifmtqtomslkqocnzuqxcjcstdrepgw";
	string ltzydovxypfvabd = "lkjjetnebiuujilslwts";
	int bfqmftdqwvgp = 4430;
	int pvascyib = 907;
	int cftwvad = 2207;
	int xzbljtlsti = 4802;
	if (4430 != 4430) {
		int lgs;
		for (lgs = 25; lgs > 0; lgs--) {
			continue;
		}
	}
	if (string("lkjjetnebiuujilslwts") == string("lkjjetnebiuujilslwts")) {
		int nsikly;
		for (nsikly = 3; nsikly > 0; nsikly--) {
			continue;
		}
	}
	if (907 != 907) {
		int uxcmsgboto;
		for (uxcmsgboto = 51; uxcmsgboto > 0; uxcmsgboto--) {
			continue;
		}
	}
	if (4802 != 4802) {
		int oeu;
		for (oeu = 83; oeu > 0; oeu--) {
			continue;
		}
	}
	return string("kowobqlzaiwtejha");
}

int jtcjslm::aqsmmihtxamotqp(bool inplvzxk, string bvxxkrtvxsuhz, double yvlxitiuj, double mmuhhawqpmmrz, string rbibbx) {
	double wmgcxkq = 24825;
	string zrielsjejy = "yqqozgctwoyvwjauweiadfejuljywyujwmhfnlcwzsbndxvr";
	int kzrvthsh = 3213;
	if (3213 != 3213) {
		int mcdheji;
		for (mcdheji = 36; mcdheji > 0; mcdheji--) {
			continue;
		}
	}
	if (3213 == 3213) {
		int jkkftytox;
		for (jkkftytox = 23; jkkftytox > 0; jkkftytox--) {
			continue;
		}
	}
	if (24825 != 24825) {
		int bqdp;
		for (bqdp = 9; bqdp > 0; bqdp--) {
			continue;
		}
	}
	if (24825 != 24825) {
		int nei;
		for (nei = 47; nei > 0; nei--) {
			continue;
		}
	}
	return 20302;
}

bool jtcjslm::mpulbedcehcsut(double pptkcyapfd, bool xuasu, string fzvrfgxv, bool wgxgdmvhhfhigv, int naqhvke, double hyxorjdfwff) {
	int ukrikmcjuahdjq = 750;
	double hmctd = 3362;
	bool cmxrjwxxheyhw = true;
	double qdafd = 46369;
	bool gfkafyorsavh = false;
	bool nqxdceaazljlgl = false;
	if (750 != 750) {
		int zzgrswk;
		for (zzgrswk = 99; zzgrswk > 0; zzgrswk--) {
			continue;
		}
	}
	if (false == false) {
		int fsm;
		for (fsm = 71; fsm > 0; fsm--) {
			continue;
		}
	}
	if (false == false) {
		int asj;
		for (asj = 78; asj > 0; asj--) {
			continue;
		}
	}
	if (true != true) {
		int hpccrh;
		for (hpccrh = 49; hpccrh > 0; hpccrh--) {
			continue;
		}
	}
	return false;
}

void jtcjslm::lbuxwrmmyjq(string fvyyilocsfnaqau, double xemhcrjtjsey, string ldofynzwbqji, string ylugeaxctpwzbl, string kheip, string xrtwbi, string imogsa) {
	double zevkrjfmgsnto = 34358;
	double nmmdzueiaxwi = 22584;
	int ixlbwvqxrcafse = 149;
	if (22584 != 22584) {
		int tbldgb;
		for (tbldgb = 71; tbldgb > 0; tbldgb--) {
			continue;
		}
	}
	if (22584 != 22584) {
		int ejripxioal;
		for (ejripxioal = 69; ejripxioal > 0; ejripxioal--) {
			continue;
		}
	}

}

double jtcjslm::ukdggqrgatrskmfozihr(bool qlusoxxjvclbmkv, double xopeigcjyzddxco, int sheti, string rtihl, bool ddcjptxmabccca, string uzoqkilnifxmiq, double njnqalojjbzsl, bool pfeiigalwmiupim, double rearz, int hxhwzicpsrs) {
	string usxwqrrt = "xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso";
	if (string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso") != string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso")) {
		int vlhkdbt;
		for (vlhkdbt = 27; vlhkdbt > 0; vlhkdbt--) {
			continue;
		}
	}
	if (string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso") != string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso")) {
		int kug;
		for (kug = 59; kug > 0; kug--) {
			continue;
		}
	}
	if (string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso") != string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso")) {
		int vrmx;
		for (vrmx = 41; vrmx > 0; vrmx--) {
			continue;
		}
	}
	if (string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso") != string("xfmmjumdjnegiqtplrkzjlitivpqpcpzqjnujmxedpqknrcutcfdqtzmrmqcwso")) {
		int xyjgd;
		for (xyjgd = 0; xyjgd > 0; xyjgd--) {
			continue;
		}
	}
	return 47422;
}

bool jtcjslm::wmbeufefwmhylcmetmthhfpxk(string eshxbvbkeleklc, int bfcjanweiydacwf, bool hesoqdvvhf, string yxrdpikkmokyq, string nsgyfkcmkvaq, int bkststieps, bool ztyqpbuglvz, string zarksrdtnp, string repiotlcrjfk) {
	int iowfzp = 1742;
	bool kxnszgjtdtifn = false;
	bool zhsfm = false;
	int qgfsrhnels = 2595;
	bool iheiexlwt = false;
	bool ibzhcr = true;
	double kkqogvtgscukoo = 3863;
	bool lhuyvrcihbw = true;
	double qmqhzzqmju = 4821;
	if (false == false) {
		int avtq;
		for (avtq = 95; avtq > 0; avtq--) {
			continue;
		}
	}
	if (2595 == 2595) {
		int dd;
		for (dd = 80; dd > 0; dd--) {
			continue;
		}
	}
	if (1742 != 1742) {
		int nnqfyrurwa;
		for (nnqfyrurwa = 41; nnqfyrurwa > 0; nnqfyrurwa--) {
			continue;
		}
	}
	if (true != true) {
		int xrcicdugwa;
		for (xrcicdugwa = 25; xrcicdugwa > 0; xrcicdugwa--) {
			continue;
		}
	}
	return false;
}

int jtcjslm::xvonexxovhqg(int iyynra, double athfqrtmjc, string fczpbwhriwpaqnu, double lguszyfcd, string epdtgvlxrnly, int nsljoeia) {
	int wapguxi = 4650;
	string yblzwytc = "pyalklvrddklrjuicbcxdnywolzkuwrpdxyldeseasdxoclrotuoylylrtglbvfn";
	int ccaecgkteip = 2627;
	bool fliexzjouv = true;
	string wjyosos = "kksycjrhjsdvalvrkmpkerozhfnmantrpjdqmbgfztldubrulmfsyjtiwer";
	double ujfmmxjtxqsxuvw = 20234;
	int ewraqpda = 1871;
	bool tcjestuffu = false;
	int vlluyndbkjicrvp = 3035;
	return 31557;
}

void jtcjslm::scfdzyflcscdmg(string vcnjjwzjerwely, double fuumyvvwtwvpndp, int fyfiwxg, bool bepljkeqechdyf, int xjgiyndk, double saqsei, double lngadfygtymr) {
	int eiwmdahfvtuimxa = 1056;
	bool zkbrmx = true;
	bool ydmecybteaepn = false;
	double akxntccrsln = 34407;

}

bool jtcjslm::aeioqrtjbhfsgxiooftcmnr(double xioiyajgenba, double ycmxt) {
	string yimbvbqk = "apgrdtigpiupkzzucrjxkyutlifnidmbzxb";
	double mjgnlbpohtbwtsu = 49776;
	string eewovuprpkn = "ymhwelldinihctmkqlvhbwkdytirbzdfcazeqpwtqr";
	int bqafgrqljdkxovr = 1814;
	int jthcirxfct = 1091;
	int gqufq = 2092;
	int lllxmjojgiu = 2527;
	if (string("apgrdtigpiupkzzucrjxkyutlifnidmbzxb") != string("apgrdtigpiupkzzucrjxkyutlifnidmbzxb")) {
		int oxvefswtgp;
		for (oxvefswtgp = 40; oxvefswtgp > 0; oxvefswtgp--) {
			continue;
		}
	}
	return false;
}

double jtcjslm::pizcywftlxwtn() {
	string uhxoygqln = "jovuquvnhaffjcjsffykjezgcxgmbzicyyzkteazqjarrstorvhedndcjlwkqsyokjhfkblpqawofasengfxgewlz";
	bool mogaxcgqlg = true;
	string gmorwudwdwg = "uqqxejzxximhbwrqbtczhcjhjqyhcu";
	double nhzwnskjba = 50266;
	int geqpnm = 1583;
	int zqutoldau = 1666;
	string fnaynrupfw = "vzmwlvehnjzrtsoeefhoxdentpszco";
	double nhvkfhvkupjt = 5453;
	return 27677;
}

bool jtcjslm::flbtoeuvgldostgw(string ntqnkieqgu) {
	bool jrwrsaxyjuw = true;
	double qvfxotca = 20892;
	if (20892 == 20892) {
		int cw;
		for (cw = 9; cw > 0; cw--) {
			continue;
		}
	}
	if (20892 != 20892) {
		int eevi;
		for (eevi = 7; eevi > 0; eevi--) {
			continue;
		}
	}
	if (20892 == 20892) {
		int eannjp;
		for (eannjp = 3; eannjp > 0; eannjp--) {
			continue;
		}
	}
	if (true == true) {
		int nr;
		for (nr = 48; nr > 0; nr--) {
			continue;
		}
	}
	if (20892 == 20892) {
		int knbjtknwk;
		for (knbjtknwk = 54; knbjtknwk > 0; knbjtknwk--) {
			continue;
		}
	}
	return false;
}

void jtcjslm::gqtaimnyexgfxsipu(int xdtdvwkk, string fqqmkldkgh, bool wajuvdlkakwxb, double voegw, int lsqgoncowsvjey, string jyqhvxsd, string ymviotbs, bool ysnuseehpjladsj, bool urbopmsgyzzt) {

}

int jtcjslm::uiwcitdjjofiixbpxijwlakit(string ghzhxolnkdmb, bool ouczjybeopd, string rshfovxtlnv, bool iiergeduodcg, string wfflfhzwwj, bool yviurkk, int evayugiookk, int mwbkgrfevawfldg) {
	return 4326;
}

double jtcjslm::tgxjsptdscmortk() {
	double rchynomcwv = 33557;
	double nzpedepqlgwic = 33255;
	return 31031;
}

jtcjslm::jtcjslm() {
	this->ukdggqrgatrskmfozihr(false, 69618, 3155, string("uuhkptszfiehxickkmnedtitmsln"), false, string("bbtjvxwagpakdtduilvmmybthbzgpojkjunjvdjsfqxeajbqlk"), 15308, false, 52054, 4455);
	this->wmbeufefwmhylcmetmthhfpxk(string("uetnablzsqunqrgixtnqfnyiunjemqysqcmkgpqsalqptzwmenyxhivbmonraogzuavnabfbhpjrjyjlqiqckltozsn"), 1699, true, string("gheyqaornxwpuysmwodyafderqvctpgoqzqwnlgo"), string("cpkvzfpbwwkwfkacxeildpshgdzbqmtcdbeikvhgemclrvhcsfsztmpyteqwjewojuyrleau"), 3248, true, string("rdmbjvorkyozuupcirtisxasgbehjrrklyvtbzreomrecjr"), string("kuiqkcjsswjouxgyirvowsbutgtjjyjnsehxzstpqexobiabiogllkikaobiyvhdjkaonyzppz"));
	this->xvonexxovhqg(2461, 8606, string("qsiossdhifqxrclexujjmbjxicngqewkntexgwxxetwmusjdegoadxaxqygurtdewkgcyyidmacj"), 20424, string("yxawuuroxbvaelxandfjxyxrsqoghwkoqhqdsadctxarscrcsduzw"), 2677);
	this->scfdzyflcscdmg(string("efitvrrfhyfrwfgwzevzbttlailuitzejslageeuntnlesmrs"), 23513, 2888, false, 2036, 1784, 23431);
	this->aeioqrtjbhfsgxiooftcmnr(27479, 1978);
	this->pizcywftlxwtn();
	this->flbtoeuvgldostgw(string("xhudofkpufeywswszqhqshinrlywsjyiywrypvgzvletegdhopgmrqwruwisumbdldwkuleukiaqwmkcxlkeuevzl"));
	this->gqtaimnyexgfxsipu(2085, string("hlvwfljwahyidynbntxeqjjxnmsljxazlqsolvuerdmennaneromyodeuxdjdrik"), false, 27863, 2570, string("qtqs"), string("jekzzvbtbsofszqrytvv"), false, true);
	this->uiwcitdjjofiixbpxijwlakit(string("xicfnuvgyroxrsfdkfohwbnlrcbwklkhynygiwpfzhgqmimnmzhyvy"), false, string("wiyuuxkmgkuehzbcywsjzggqwxjzgbymyankdqpobkmvenxuhrsujppfrhleqelsoyetupn"), true, string("skuitsqsqklndjbsnztzyclikleylhxujsgeipeyppafnrqgjbdwmdbckqpleyvbnzakivpcfnnoptdvlemttkrf"), false, 3292, 452);
	this->tgxjsptdscmortk();
	this->aqsmmihtxamotqp(false, string("nqqdrdydawrelosktsjgwsbjeqvvuqbkfwcotellrsernutpgiogemfwspxckryvzntkjrahmtwvyydwqfvqahhc"), 34916, 43613, string("uwlmmeroadxgluhwfin"));
	this->mpulbedcehcsut(21063, true, string("zqiwjmfowmgkdgnhufvmogoikzcdutvawgamgbtqmllwuxlckjrpdfpmggtipvbyqutzvcfjapfyvirxk"), false, 6531, 46417);
	this->lbuxwrmmyjq(string("phgqmjtobwndpvejftqfdnnnhyjpdkerqbkujthuctspvhbgkiomfdaottdgbkmjozdwokuvvqkyvqypedxhfqwsjfytxmawfqy"), 54526, string("ieioctgfnilkyzitwvgmeujewpptgdqlxfqzrpjyywqpnpxksqusolnaqgxwlpsyellspgxtcxurhdktqvfmqixoeqeavqylj"), string("cmzqidkyxdeoktzyqrvevlxqmdmnnymamrvcsh"), string("yonxzzvprmpfndogdbwjooikbrlaeqjspkjmhmlysmiso"), string("jnrquxikpyelid"), string("eeaadvuhjnjtlzdsnulnynrznjsovwirgvrtmu"));
	this->hkmeegzjllc(true, 10059, 35439, 45732);
}
