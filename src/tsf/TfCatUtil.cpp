/*++

Copyright (c) Microsoft Corporation.
Licensed under the MIT license.

Module Name:

    TfCatUtil.cpp

Abstract:

    This file implements the CicCategoryMgr Class.

Author:

Revision History:

Notes:

--*/

#include "precomp.h"
#include "TfCatUtil.h"

[[nodiscard]] HRESULT CicCategoryMgr::GetGUIDFromGUIDATOM(TfGuidAtom guidatom, GUID* pguid)
{
    return m_pcat->GetGUID(guidatom, pguid);
}

[[nodiscard]] HRESULT CicCategoryMgr::InitCategoryInstance()
{
    //
    // Create ITfCategoryMgr instance.
    //
    return ::CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&m_pcat));
}

ITfCategoryMgr* CicCategoryMgr::GetCategoryMgr()
{
    return m_pcat.get();
}
