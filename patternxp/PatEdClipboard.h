#pragma once

#include "stdafx.h"
#include "App.h"
#include "ActionStack.h"
#include <functional>
#include "../../buzz/MachineInterface.h"

#define PXP_CLIPBOARD_FORMAT "PXP_FORMAT"

class PatEdClipboard
{
public:
	static void CopyToClipBoard(CMachineInterface * mi)
	{
		UINT format = RegisterClipboardFormat(PXP_CLIPBOARD_FORMAT);

		ByteVector bv;
		CMemDataOutput mdo(&bv);

		mi->Save(&mdo);

		OpenClipboard(0);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, bv.size());
		if (!hg) {
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), bv.data(), bv.size());
		GlobalUnlock(hg);
		SetClipboardData(format, hg);
		CloseClipboard();
		GlobalFree(hg);
	};

	static void CopyFromClipBoard(CMachineInterface* mi, CMachine* pMachine)
	{
		UINT format = RegisterClipboardFormat(PXP_CLIPBOARD_FORMAT);

		ByteVector bv;
		CMemDataInput mdi(&bv);
		CMachineDataInput* pi = (CMachineDataInput*)&mdi;

		OpenClipboard(0);

		HGLOBAL hg = GetClipboardData(format);
		if (!hg) {
			CloseClipboard();
			return;
		}
		int size = GlobalSize(hg);

		bv.resize(size);
		memcpy(bv.data(), GlobalLock(hg), size);
		GlobalUnlock(hg);

		CloseClipboard();
	
		// mi->Init(&mdi);
		// 
		// ToDo: Call all sorts of MachineInterface functions
		// 
		// 1. Delete all machine patterns
		// 2. Create new patterns using data from mdi
		// 3. ...

	};

private:
	
};