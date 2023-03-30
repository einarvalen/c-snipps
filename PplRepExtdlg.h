// $Revision: 2 $
// $Archive: /gemini-maintenence/Client/srchext/PplRepExt/filedlg.h $

//---------------------------------------------------------------------------
//
//	filedlg.h
//
//	Declarations for the dialog which allows the user to input options
//	for File Search Extension.
//
//	5/27/98   Copyright Actuate Software Corporation
//
//---------------------------------------------------------------------------

#ifndef __FILEDLG_H__
#define __FILEDLG_H__

class PplRepExtOptions : public CDialog
{
	DECLARE_DYNCREATE(PplRepExtOptions)

// Construction
public:
	PplRepExtOptions();
	PplRepExtOptions( CWnd *parentWin );
	~PplRepExtOptions();

	BOOL	OptionsChanged();

// Members

// Dialog Data
	//{{AFX_DATA(PplRepExtOptions)
//	enum { IDD = IDD_FILE_OPTIONS };
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_colDelimiter;
	CString		m_dataFile;
	CButton		m_dataBrowse;
	CString		m_cmdLine;
	BOOL		m_headers;
	//}}AFX_DATA

	CString		m_colDelimVal;

protected:
	void		SetDefaults();

	// Column delimiter methods.
	void		SyncDelimiter();
	CString		TranslateString( CString escapedString );

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PplRepExtOptions)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PplRepExtOptions)
	virtual BOOL OnInitDialog();
    virtual void OnOK();
	afx_msg void OnDataBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __FILEDLG_H__
