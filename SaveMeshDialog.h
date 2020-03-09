#pragma once


// SaveMeshDialog dialog

class SaveMeshDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SaveMeshDialog)

public:
	SaveMeshDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SaveMeshDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
