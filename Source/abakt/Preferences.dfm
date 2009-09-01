object f_Prefs: Tf_Prefs
  Left = 408
  Top = 224
  Width = 440
  Height = 340
  BorderIcons = [biSystemMenu]
  Caption = 'Preferences'
  Color = clBtnFace
  Constraints.MinHeight = 340
  Constraints.MinWidth = 440
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 432
    Height = 269
    ActivePage = tab_Application
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object tab_Startup: TTabSheet
      Caption = 'On &Startup'
      DesignSize = (
        424
        241)
      object Label4: TLabel
        Left = 26
        Top = 129
        Width = 390
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Caption = 
          'Applies to '#39'.abp'#39', '#39'.abg'#39', '#39'.abt'#39' and '#39'.abh'#39' files. Will be perf' +
          'ormed once, at the next startup.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object cb_CreateFileAssociations: TCheckBox
        Left = 8
        Top = 112
        Width = 409
        Height = 16
        Caption = '&Restore File Associations at Next Startup'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object rb_InitialProfileType: TRadioGroup
        Left = 8
        Top = 8
        Width = 409
        Height = 89
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Use This Profile'
        ItemIndex = 1
        Items.Strings = (
          '&New'
          '&Previously Opened/Saved'
          'C&ustom')
        TabOrder = 0
      end
      object bt_SelectCustomInitialProfile: TButton
        Left = 334
        Top = 69
        Width = 73
        Height = 21
        Anchors = [akTop, akRight]
        Caption = '&Browse'
        TabOrder = 1
        OnClick = bt_SelectCustomInitialProfileClick
      end
      object ed_CustomInitialProfile: TEdit
        Left = 112
        Top = 69
        Width = 209
        Height = 20
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        TabOrder = 2
      end
    end
    object tab_Application: TTabSheet
      Caption = '&Application'
      ImageIndex = 1
      DesignSize = (
        424
        241)
      object cb_ShowDescriptionBoxes: TCheckBox
        Left = 8
        Top = 213
        Width = 169
        Height = 17
        Caption = 'Show Description &Boxes'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
      object Panel8: TPanel
        Left = 208
        Top = 211
        Width = 209
        Height = 22
        Anchors = [akLeft, akTop, akRight]
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 5
        object Label13: TLabel
          Left = 2
          Top = 2
          Width = 205
          Height = 18
          Align = alClient
          Alignment = taCenter
          Caption = 'This is a Description Box.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
        end
      end
      object gbx_Language: TGroupBox
        Left = 8
        Top = 117
        Width = 409
        Height = 44
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Lang&uage'
        TabOrder = 2
        DesignSize = (
          409
          44)
        object Label14: TLabel
          Left = 200
          Top = 11
          Width = 201
          Height = 30
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Caption = 'Requires application restart.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
        object cmbx_Languages: TComboBox
          Left = 8
          Top = 16
          Width = 185
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 163
        Width = 409
        Height = 41
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Preferences File'
        TabOrder = 3
        DesignSize = (
          409
          41)
        object ed_PrefsFile: TEdit
          Left = 8
          Top = 16
          Width = 394
          Height = 19
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 0
          Text = 'ed_PrefsFile'
        end
      end
      object gbx_RecentFiles: TGroupBox
        Left = 8
        Top = 54
        Width = 409
        Height = 61
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Recent Files Menu'
        TabOrder = 1
        object Label2: TLabel
          Left = 8
          Top = 20
          Width = 104
          Height = 13
          Caption = 'Numbe&r of Listed Files'
          FocusControl = ed_MaxRecentFiles
        end
        object cb_RecentFilesShowPath: TCheckBox
          Left = 6
          Top = 40
          Width = 207
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Show &Path Information'
          TabOrder = 2
        end
        object ed_MaxRecentFiles: TEdit
          Left = 200
          Top = 16
          Width = 25
          Height = 21
          TabOrder = 1
          Text = '9'
        end
        object ud_MaxRecentFiles: TUpDown
          Left = 225
          Top = 16
          Width = 15
          Height = 21
          Associate = ed_MaxRecentFiles
          Min = 0
          Max = 32
          Position = 9
          TabOrder = 0
          Wrap = False
        end
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 8
        Width = 409
        Height = 44
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Data Directory'
        TabOrder = 0
        DesignSize = (
          409
          44)
        object ed_DefDataDir: TEdit
          Left = 8
          Top = 16
          Width = 360
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object bt_DefDataDir: TButton
          Left = 376
          Top = 16
          Width = 25
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 0
          OnClick = bt_DefDataDirClick
        end
      end
    end
    object tab_Backup: TTabSheet
      Caption = 'A&ction'
      ImageIndex = 3
      DesignSize = (
        424
        241)
      object Label7: TLabel
        Left = 26
        Top = 206
        Width = 391
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Caption = 'Not used when Abakt is started with command line parameters.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object cb_ActFile_ConfirmDelete: TCheckBox
        Left = 8
        Top = 188
        Width = 409
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Confirm '#39'File Operation | Delete'#39
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object gbx_DefaultDestDir: TGroupBox
        Left = 8
        Top = 8
        Width = 409
        Height = 44
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Default &Destination Directory'
        TabOrder = 0
        DesignSize = (
          409
          44)
        object ed_DefDestDir: TEdit
          Left = 8
          Top = 16
          Width = 360
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object bt_DefDestDir: TButton
          Left = 376
          Top = 16
          Width = 25
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = bt_DefDestDirClick
        end
      end
      object gbx_TemporaryDirectory: TGroupBox
        Left = 8
        Top = 54
        Width = 409
        Height = 44
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Temporary Directory'
        TabOrder = 1
        DesignSize = (
          409
          44)
        object ed_TempDir: TEdit
          Left = 8
          Top = 16
          Width = 273
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object bt_TempDir: TButton
          Left = 376
          Top = 16
          Width = 25
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 0
          OnClick = bt_TempDirClick
        end
        object bt_DefaultTempDir: TButton
          Left = 290
          Top = 16
          Width = 79
          Height = 21
          Anchors = [akTop, akRight]
          Caption = 'Default'
          TabOrder = 2
          OnClick = bt_DefaultTempDirClick
        end
      end
      object gbx_7ZipExe: TGroupBox
        Left = 8
        Top = 100
        Width = 409
        Height = 81
        Anchors = [akLeft, akTop, akRight]
        Caption = '&7-Zip Executable'
        TabOrder = 2
        DesignSize = (
          409
          81)
        object lbl_Get7Zip: TLabel
          Left = 8
          Top = 60
          Width = 269
          Height = 13
          Caption = 'Get 7-Zip from http://sourceforge.net/projects/sevenzip/'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnClick = lbl_Get7ZipClick
        end
        object bt_7ZipExe: TButton
          Left = 376
          Top = 16
          Width = 25
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 0
          OnClick = bt_7ZipExeClick
        end
        object ed_ExePath7Zip: TEdit
          Left = 8
          Top = 16
          Width = 360
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object cb_OEMlistfile7Zip: TCheckBox
          Left = 8
          Top = 40
          Width = 385
          Height = 17
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Convert filenames in @listfile to OEM'
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
      end
    end
    object tab_Logging: TTabSheet
      Caption = '&Logging'
      ImageIndex = 2
      DesignSize = (
        424
        241)
      object gbx_Logging_Action: TGroupBox
        Left = 8
        Top = 8
        Width = 409
        Height = 153
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Action'
        TabOrder = 0
        DesignSize = (
          409
          153)
        object Label3: TLabel
          Left = 26
          Top = 32
          Width = 375
          Height = 24
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Caption = 'For every file: show IF and WHY it is included or excluded.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label10: TLabel
          Left = 26
          Top = 72
          Width = 375
          Height = 26
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Caption = 
            'Show detailed progress for every file. If unchecked, only the fa' +
            'ilures are reported.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object cb_LogBuildingFileLists: TCheckBox
          Left = 8
          Top = 16
          Width = 385
          Height = 16
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Detailed &Building File Lists'
          TabOrder = 0
        end
        object cb_LogFullAction: TCheckBox
          Left = 8
          Top = 56
          Width = 385
          Height = 16
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Detailed &Progress'
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
        object bt_ActionLogDir: TButton
          Left = 26
          Top = 123
          Width = 95
          Height = 23
          Caption = '&Directory'
          TabOrder = 3
          OnClick = bt_ActionLogDirClick
        end
        object ed_ActionLogDir: TEdit
          Left = 128
          Top = 125
          Width = 273
          Height = 19
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 4
        end
        object cb_AutoSaveActionLog: TCheckBox
          Left = 8
          Top = 105
          Width = 385
          Height = 16
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Save Log &to "<ProfileName>.log"'
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
      end
      object gbx_Logging_Application: TGroupBox
        Left = 8
        Top = 164
        Width = 410
        Height = 63
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Application'
        TabOrder = 1
        DesignSize = (
          410
          63)
        object Label8: TLabel
          Left = 26
          Top = 37
          Width = 92
          Height = 13
          Caption = 'Maximum Size (kB):'
        end
        object cb_SaveAppStatus: TCheckBox
          Left = 8
          Top = 16
          Width = 370
          Height = 15
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Save Status &Messages'
          Checked = True
          State = cbChecked
          TabOrder = 0
        end
        object ed_AppStatusMaxSize: TEdit
          Left = 178
          Top = 34
          Width = 51
          Height = 21
          TabOrder = 1
          Text = '2048'
        end
        object sped_MaxStatusLogSize: TUpDown
          Left = 229
          Top = 34
          Width = 17
          Height = 21
          Associate = ed_AppStatusMaxSize
          Min = 256
          Max = 4096
          Position = 2048
          TabOrder = 2
          Thousands = False
          Wrap = False
        end
      end
    end
    object tab_Advanced: TTabSheet
      Caption = 'Ad&vanced'
      ImageIndex = 4
      DesignSize = (
        424
        241)
      object Label9: TLabel
        Left = 26
        Top = 138
        Width = 391
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Caption = 
          'Make sure that only 100% successful backups are made. Applies to' +
          ' Zip Actions only.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object Label5: TLabel
        Left = 26
        Top = 50
        Width = 391
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Caption = 
          'Applies to Zip Method '#39'Replace'#39'. Always disabled for removable d' +
          'rives.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object Label1: TLabel
        Left = 26
        Top = 94
        Width = 391
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Caption = 
          'Instructs ZipBuilder to work on a temporary copy of existing zip' +
          ' archive.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object cb_RemoveNonExistingSourceDirs: TCheckBox
        Left = 8
        Top = 8
        Width = 409
        Height = 16
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Remove Non-Existing Source Directories from Profile'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object cb_UseNewUIOnBFD: TCheckBox
        Left = 8
        Top = 168
        Width = 409
        Height = 16
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Use New UI BrowseFor&Folder'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
      object cb_CreateTempZipCopy: TCheckBox
        Left = 8
        Top = 34
        Width = 409
        Height = 16
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Create &Temporary Copy if Zip Archive Exists'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object cb_TreatWarningsAsErrors: TCheckBox
        Left = 8
        Top = 122
        Width = 409
        Height = 16
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Treat &Warnings as Errors'
        TabOrder = 3
      end
      object cb_UseZipBuilderAddSafe: TCheckBox
        Left = 8
        Top = 78
        Width = 409
        Height = 16
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Perform Safe ZipBuilder Operations'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
    end
  end
  object pnl_Bottom: TPanel
    Left = 0
    Top = 269
    Width = 432
    Height = 38
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      432
      38)
    object bt_Help: TButton
      Left = 8
      Top = 8
      Width = 72
      Height = 25
      Caption = 'Help'
      TabOrder = 0
      OnClick = bt_HelpClick
    end
    object bt_Ok: TButton
      Left = 272
      Top = 8
      Width = 72
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = bt_OkClick
    end
    object bt_Cancel: TButton
      Left = 352
      Top = 8
      Width = 72
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
      OnClick = bt_CancelClick
    end
  end
  object odlg_7ZipExe: TOpenDialog
    DefaultExt = 'exe'
    FileName = '7z.exe'
    Filter = 'Application (*.exe)|*.exe|Any File (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofDontAddToRecent]
    Title = '7-Zip Commandline Executable'
    Left = 208
    Top = 264
  end
  object odlg_InitialProfile: TOpenDialog
    DefaultExt = 'abp'
    Filter = 'Abakt Profile (*.abp)|*.abp|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofExtensionDifferent, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofForceShowHidden]
    Left = 120
    Top = 264
  end
end
