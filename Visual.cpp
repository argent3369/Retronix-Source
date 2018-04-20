#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Esp.h"
#include "DLLMain.h"



void CVisuals::Init()
{

}

void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

void CVisuals::Draw()
{

	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();

	if (Menu::Window.VisualsTab.Normalcrosshair.GetState())
		DrawCrosshair();

	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();

	if (Menu::Window.VisualsTab.SpreadCrosshair.GetState())
		SpreadCrosshair();

	if (Menu::Window.VisualsTab.AALines.GetState())
		bighak();


	switch (Menu::Window.VisualsTab.XHairs.GetIndex())
	{
	case 0:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		break;
	case 1:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		DrawRecoilCrosshair();
		break;
	case 2:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		SpreadCrosshair();
		break;
	}
}

void CVisuals::NoScopeCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GameUtils::IsSniper(pWeapon))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 255));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 255));
	}
}
void CVisuals::bighak()
{
	/*	IClientEntity *pLocal = hackManager.pLocal();
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;
	AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
	return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255));
	Render::Text(dst.x, dst.y, Color(255, 165.f, 0, 255), Render::Fonts::ESP, "LBY");*/

	IClientEntity *pLocal = hackManager.pLocal();

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = hackManager.pLocal();

	AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = hackManager.pLocal()->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 255, 0, 255));
	Render::Text(dst.x, dst.y, Color(255, 165.f, 0, 255), Render::Fonts::ESP, "lby");

	AngleVectors(QAngle(0, lineRealAngle, 0), &forward);
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(0, 70, 230, 255));
	Render::Text(dst.x, dst.y, Color(0, 70.f, 230, 255), Render::Fonts::ESP, "real");

	AngleVectors(QAngle(0, lineFakeAngle, 0), &forward);
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 255, 255));
	Render::Text(dst.x, dst.y, Color(255, 0.f, 0, 255), Render::Fonts::ESP, "fake");
}
void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	IClientEntity* WeaponEnt = Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!hackManager.pLocal()->IsAlive())
		return;

	if (!GameUtils::IsBallisticWeapon(pWeapon))
		return;
	if (pWeapon == nullptr)
		return;
	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	auto accuracy = pWeapon->GetInaccuracy() * 550.f; //3000
	Render::DrawFilledCircle(Vector2D(xs, ys), Color(70, 70, 70, 89), accuracy, 60);
	if (pLocal->IsAlive())
	{
		if (pWeapon)
		{

			float inaccuracy = pWeapon->GetInaccuracy() * 1000;
			char buffer4[64];
			//	sprintf_s(buffer4, "Spread:  %.00f", inaccuracy);

			//if (inaccuracy < 15)
			//	Render::Text(xs + accuracy + 4, ys, Color(5, 255, 80, 255), Render::Fonts::ESP, buffer4);
			//else if (inaccuracy >= 15 && inaccuracy < 100)
			//Render::Text(xs + accuracy + 4, ys, Color(255, 160, 20, 255), Render::Fonts::ESP, buffer4);
			//	else
			//Render::Text(xs + accuracy + 4, ys, Color(255, 10, 110, 255), Render::Fonts::ESP, buffer4);

		}

	}
	else
	{

		//	Render::Text(10, 70, Color(180, 20, 255, 255), Render::Fonts::ESP, "Spread: --");
	}

}


void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 4, MidY - 4, MidX + 4, MidY + 4, Color(0, 255, 0, 255));
	Render::Line(MidX + 4, MidY - 4, MidX - 4, MidY + 4, Color(0, 255, 0, 255));
}





void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 10, endScreen.y, endScreen.x + 10, endScreen.y, Color(0, 255, 0, 255));
		Render::Line(endScreen.x, endScreen.y - 10, endScreen.x, endScreen.y + 10, Color(0, 255, 0, 255));
	}
}



