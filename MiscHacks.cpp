#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;



void CMiscHacks::Init()
{

}

void CMiscHacks::Draw()
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

    	if (Menu::Window.MiscTab.NoName.GetState())
		    NoName();

		if (Menu::Window.MiscTab.Namespam.GetState())
		    Namespam();



};



void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{

	if (Menu::Window.VisualsTab.Logs.GetState())
	{

		ConVar* Developer = Interfaces::CVar->FindVar("developer");
		*(float*)((DWORD)&Developer->fnChangeCallback + 0xC) = NULL;
		Developer->SetValue("1");

		ConVar* con_filter_enable = Interfaces::CVar->FindVar("con_filter_enable");
		*(float*)((DWORD)&con_filter_enable->fnChangeCallback + 0xC) = NULL;
		con_filter_enable->SetValue("2");

		ConVar* con_filter_text = Interfaces::CVar->FindVar("con_filter_text");
		*(float*)((DWORD)&con_filter_text->fnChangeCallback + 0xC) = NULL;
		con_filter_text->SetValue("[$Argent$]");
	}


	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.MiscTab.OtherAutoJump.GetState())
		AutoJump(pCmd);


	Fakelag(pCmd, bSendPacket);


	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		break;
	case 1:
		LegitStrafe(pCmd);
		break;

	case 2:
		RageStrafe(pCmd);
		break;

	}

	
	

	if (Menu::Window.MiscTab.AutoPistol.GetState())
		AutoPistol(pCmd);

	if (Menu::Window.MiscTab.MoonWalk.GetState())
		MoonWalk(pCmd);

	if (Menu::Window.MiscTab.OtherCircle.GetState())
		CircularStrafe(pCmd, pCmd->viewangles);

	if (Menu::Window.MiscTab.DisablePostProcess.GetState())
		PostProcces();

	if (Menu::Window.MiscTab.Impacts.GetState())
		Impacts();

	

	//IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (Menu::Window.RageBotTab.WeaponCheck.GetState())
	{

		if (GameUtils::IsScout(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.scoutmindmg.GetValue());
		}
		else if (GameUtils::IsAWP(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.AWPmindmg.GetValue());
		}
		else if (GameUtils::IsAuto(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Automindmg.GetValue());
		}
		else if (GameUtils::IsR8(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.R8mindmg.GetValue());
		}
		else if (GameUtils::IsPistol(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Pistolmindmg.GetValue());
		}
		else if (GameUtils::IsRifle(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Riflemindmg.GetValue());
		}
		else if (GameUtils::IsMP(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.MPmindmg.GetValue());
		}





		if (GameUtils::IsScout(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.scoutChance.GetValue());
		}
		else if (GameUtils::IsAWP(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.AWPChance.GetValue());
		}
		else if (GameUtils::IsAuto(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.AutoChance.GetValue());
		}
		else if (GameUtils::IsR8(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.R8Chance.GetValue());
		}
		else if (GameUtils::IsPistol(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.PistolChance.GetValue());
		}
		else if (GameUtils::IsRifle(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.RifleChance.GetValue());
		}
		else if (GameUtils::IsMP(pWeapon))
		{

			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.MPChance.GetValue());
		}


	}
}


void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}

void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::Impacts()
{
	ConVar* impacts = Interfaces::CVar->FindVar("sv_showimpacts");
	SpoofedConvar* impacts_spoofed = new SpoofedConvar(impacts);
	impacts_spoofed->SetString("sv_showimpacts 3");
}

void CMiscHacks::AutoPistol(CUserCmd* pCmd)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pWeapon)
	{

		if (GameUtils::IsBomb(pWeapon))
		{
			return;
		}
		if (!GameUtils::IsNotPistol(pWeapon))
		{
			return;
		}
		if (GameUtils::IsGrenade(pWeapon))
		{
			return;
		}

	}
	static bool WasFiring = false;

	if (GameUtils::IsPistol)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
}

void CMiscHacks::RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

bool CMiscHacks::CircularStrafe(CUserCmd* pCmd, Vector& OriginalView)
{
	if (!(Menu::Window.MiscTab.OtherCirclekey.GetKey()))
		return false;

	IClientEntity* pLocalEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity)
		return false;

	if (!pLocalEntity->IsAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 360)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	int CIRCLEKey = Menu::Window.MiscTab.OtherCirclekey.GetKey();

	if (CIRCLEKey > 0 && GUI.GetKeyState(CIRCLEKey))
	{
		pCmd->viewangles = OriginalView;
		RotateMovement(pCmd, GetItDoubled);
	}
	return true;
}

void CMiscHacks::Namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("Щ$Argent$");
			useSpace = !useSpace;
		}
		else
		{
			change_name("$Argent$Щ");
			useSpace = !useSpace;
		}

	}

	start_t = clock();
}


void CMiscHacks::NoName()
{

	change_name("\nннн");
}




void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}

	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}

	}
}

void CMiscHacks::MoonWalk(CUserCmd *pCmd)
{
	if (pCmd->forwardmove > 0)
	{
		pCmd->buttons |= IN_BACK;
		pCmd->buttons &= ~IN_FORWARD;
	}

	if (pCmd->forwardmove < 0)
	{
		pCmd->buttons |= IN_FORWARD;
		pCmd->buttons &= ~IN_BACK;
	}

	if (pCmd->sidemove < 0)
	{
		pCmd->buttons |= IN_MOVERIGHT;
		pCmd->buttons &= ~IN_MOVELEFT;
	}

	if (pCmd->sidemove > 0)
	{
		pCmd->buttons |= IN_MOVELEFT;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{

	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static bool bDirection = true;

	static float move = 450.f;
	float s_move = move * 0.5065f;
	static float strafe = pCmd->viewangles.y;
	float rt = pCmd->viewangles.y, rotation;

	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{

		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		rotation = strafe - rt;

		strafe = rt;

		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;

		bool bKeysPressed = true;

		if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44))
			bKeysPressed = false;
		if (pCmd->buttons & IN_ATTACK)
			bKeysPressed = false;

		float flYawBhop = 0.f;

		float sdmw = pCmd->sidemove;
		float fdmw = pCmd->forwardmove;

		static float move = 450.f;
		float s_move = move * 0.5276f;
		static float strafe = pCmd->viewangles.y;

		if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 2 && !GetAsyncKeyState(VK_RBUTTON))
		{
			if (pLocal->GetVelocity().Length() > 45.f)
			{
				float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

				z = x / y;
				z = fabsf(z);

				a = x * z;

				flYawBhop = a;
			}

			if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
			{

				if (bDirection)
				{
					AutoStrafeView -= flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = -450;
					bDirection = false;
				}
				else
				{
					AutoStrafeView += flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = 430;
					bDirection = true;
				}

				if (pCmd->mousedx < 0)
				{
					pCmd->forwardmove = 22;
					pCmd->sidemove = -450;
				}

				if (pCmd->mousedx > 0)
				{
					pCmd->forwardmove = +22;
					pCmd->sidemove = 450;
				}
			}
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

