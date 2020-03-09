// SaveMeshDialog.cpp : implementation file
//

#include "stdafx.h"
// #include "pch.h"
#include "SaveMeshDialog.h"
#include "afxdialogex.h"


// SaveMeshDialog dialog

IMPLEMENT_DYNAMIC(SaveMeshDialog, CDialogEx)

SaveMeshDialog::SaveMeshDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAVEMESH, pParent)
{

}

SaveMeshDialog::~SaveMeshDialog()
{
}

void SaveMeshDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SaveMeshDialog, CDialogEx)
END_MESSAGE_MAP()


// SaveMeshDialog message handlers
