object FrameActionTypeSettings: TFrameActionTypeSettings
  Left = 0
  Top = 0
  Width = 570
  Height = 439
  AutoScroll = False
  TabOrder = 0
  object pgc_ActionTypes: TPageControl
    Left = 0
    Top = 20
    Width = 570
    Height = 419
    ActivePage = tab_File
    Align = alClient
    Images = DM.imgs_CheckBoxes
    MultiLine = True
    Style = tsFlatButtons
    TabHeight = 20
    TabIndex = 2
    TabOrder = 0
    OnChange = pgc_ActionTypesChange
    object tab_Zip: TTabSheet
      Caption = '&Zip'
      ImageIndex = -1
      object ScrollBox_ActZip: TScrollBox
        Left = 0
        Top = 0
        Width = 562
        Height = 389
        VertScrollBar.Tracking = True
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        TabOrder = 0
        object gbx_Zip_Method: TGroupBox
          Left = 0
          Top = 20
          Width = 558
          Height = 87
          Align = alTop
          Caption = '&Method'
          TabOrder = 0
          object Label10: TLabel
            Left = 104
            Top = 33
            Width = 264
            Height = 13
            Caption = 'Existing backup with the same filename will be replaced.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label9: TLabel
            Left = 104
            Top = 50
            Width = 417
            Height = 13
            Caption = 
              'Add new files and update files already present in existing backu' +
              'p with the same filename.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label3: TLabel
            Left = 104
            Top = 16
            Width = 265
            Height = 13
            Caption = 'Never overwrite existing backup with the same filename.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label4: TLabel
            Left = 104
            Top = 67
            Width = 427
            Height = 13
            Caption = 
              'Update files already present, don'#39't add new files to existing ba' +
              'ckup with the same filename.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object rb_Zip_MethodReplace: TRadioButton
            Left = 8
            Top = 32
            Width = 96
            Height = 17
            Caption = 'Replace'
            TabOrder = 1
            OnClick = SetProfileModified
          end
          object rb_Zip_MethodUpdate: TRadioButton
            Left = 8
            Top = 49
            Width = 96
            Height = 17
            Caption = 'Update'
            TabOrder = 2
            OnClick = SetProfileModified
          end
          object rb_Zip_MethodNew: TRadioButton
            Left = 8
            Top = 15
            Width = 96
            Height = 17
            Caption = 'New'
            Checked = True
            TabOrder = 0
            TabStop = True
            OnClick = SetProfileModified
          end
          object rb_Zip_MethodFreshen: TRadioButton
            Left = 8
            Top = 66
            Width = 96
            Height = 17
            Caption = 'Freshen'
            TabOrder = 3
            OnClick = SetProfileModified
          end
        end
        object pnl_Split: TPanel
          Left = 0
          Top = 165
          Width = 558
          Height = 68
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 3
          DesignSize = (
            558
            68)
          object Label11: TLabel
            Left = 27
            Top = 26
            Width = 58
            Height = 13
            Caption = 'Vo&lume Size'
            FocusControl = cmbx_Zip_SplitVolumeSizePreset
          end
          object Label12: TLabel
            Left = 26
            Top = 46
            Width = 74
            Height = 13
            Caption = 'Custom &Size (B)'
            FocusControl = bt_actZip_SplitCustomSize
          end
          object lbl_ZipInfo_SplitCustomSize: TLabel
            Left = 278
            Top = 46
            Width = 108
            Height = 13
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object cb_Zip_Split: TCheckBox
            Left = 8
            Top = 4
            Width = 145
            Height = 17
            Caption = 'Spl&it Archive'
            TabOrder = 0
            OnClick = SetProfileModified
          end
          object cmbx_Zip_SplitVolumeSizePreset: TComboBox
            Left = 160
            Top = 20
            Width = 385
            Height = 21
            Style = csDropDownList
            Anchors = [akLeft, akTop, akRight]
            ItemHeight = 13
            TabOrder = 1
            OnSelect = SetProfileModified
          end
          object ed_Zip_SplitCustomSize: TEdit
            Left = 160
            Top = 43
            Width = 87
            Height = 21
            TabOrder = 2
            Text = '65536'
            OnChange = ed_Zip_SplitCustomSizeChange
            OnDblClick = ed_Zip_SplitCustomSizeDblClick
            OnKeyPress = ed_Zip_SplitCustomSizeKeyPress
          end
          object bt_actZip_SplitCustomSize: TButton
            Left = 250
            Top = 43
            Width = 20
            Height = 21
            Caption = '...'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = bt_actZip_SplitCustomSizeClick
          end
        end
        object pnl_Zip_Comment: TPanel
          Left = 0
          Top = 262
          Width = 558
          Height = 49
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 5
          DesignSize = (
            558
            49)
          object Label6: TLabel
            Left = 27
            Top = 27
            Width = 44
            Height = 13
            Caption = 'Template'
            FocusControl = cmbx_Zip_SplitVolumeSizePreset
          end
          object cb_Zip_AddComment: TCheckBox
            Left = 8
            Top = 4
            Width = 537
            Height = 17
            Anchors = [akLeft, akTop, akRight]
            Caption = 'Add Comment'
            TabOrder = 0
            OnClick = SetProfileModified
          end
          object ed_Zip_CommentTemplate: TEdit
            Left = 112
            Top = 24
            Width = 307
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            AutoSize = False
            BevelEdges = [beLeft, beTop, beRight]
            TabOrder = 1
            Text = #39'Created with Abakt $V on '#39'yyyy'#39'-'#39'mm'#39'-'#39'dd'#39'@'#39'hh.nn'
            OnChange = SetProfileModified
          end
          object bt_Zip_CommentPreview: TButton
            Left = 430
            Top = 24
            Width = 115
            Height = 21
            Anchors = [akTop, akRight]
            Caption = 'Preview'
            TabOrder = 2
            OnClick = bt_Zip_CommentPreviewClick
          end
        end
        object pnl_Descr_actZip: TPanel
          Left = 0
          Top = 0
          Width = 558
          Height = 20
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 6
          object Label15: TLabel
            Left = 8
            Top = 3
            Width = 186
            Height = 13
            Caption = 'Copy the selected files to a Zip archive.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
        end
        object Panel2: TPanel
          Left = 0
          Top = 233
          Width = 558
          Height = 29
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 4
          object Label26: TLabel
            Left = 8
            Top = 7
            Width = 46
            Height = 13
            Caption = 'Pass&word'
            FocusControl = ed_Zip_Password
          end
          object ed_Zip_Password: TEdit
            Left = 112
            Top = 4
            Width = 225
            Height = 21
            AutoSize = False
            MaxLength = 32
            PasswordChar = '*'
            TabOrder = 0
            OnChange = SetProfileModified
          end
        end
        object pnl_Compression: TPanel
          Left = 0
          Top = 136
          Width = 558
          Height = 29
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 2
          object Label1: TLabel
            Left = 8
            Top = 7
            Width = 60
            Height = 13
            Caption = '&Compression'
            FocusControl = ed_Zip_Compression
          end
          object lbl_ZipInfo_Compression: TLabel
            Left = 248
            Top = 8
            Width = 108
            Height = 13
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object ed_Zip_Compression: TEdit
            Left = 112
            Top = 4
            Width = 24
            Height = 21
            MaxLength = 1
            TabOrder = 0
            Text = '6'
            OnChange = ed_Zip_CompressionChange
            OnKeyPress = ed_Zip_CompressionKeyPress
          end
          object ud_Zip_Compression: TUpDown
            Left = 136
            Top = 4
            Width = 16
            Height = 21
            Associate = ed_Zip_Compression
            Min = 0
            Max = 9
            Position = 6
            TabOrder = 1
            Thousands = False
            Wrap = False
            OnChangingEx = ud_Zip_CompressionChangingEx
          end
        end
        object pnl_StorePath: TPanel
          Left = 0
          Top = 107
          Width = 558
          Height = 29
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 1
          object lbl_ZipInfo_StorePath: TLabel
            Left = 248
            Top = 8
            Width = 108
            Height = 13
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label8: TLabel
            Left = 8
            Top = 7
            Width = 77
            Height = 13
            Caption = '&Path Information'
            FocusControl = cmbx_Zip_StorePath
          end
          object cmbx_Zip_StorePath: TComboBox
            Left = 112
            Top = 4
            Width = 129
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 0
            OnSelect = SetProfileModified
          end
        end
      end
    end
    object tab_7Zip: TTabSheet
      Caption = '&7-Zip'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImageIndex = -1
      ParentFont = False
      object ScrollBox_7Zip: TScrollBox
        Left = 0
        Top = 0
        Width = 562
        Height = 389
        VertScrollBar.Tracking = True
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        TabOrder = 0
        object gbx_7z_CommandLine: TGroupBox
          Left = 0
          Top = 20
          Width = 558
          Height = 89
          Align = alTop
          Caption = 'Command Line'
          TabOrder = 0
          DesignSize = (
            558
            89)
          object Label22: TLabel
            Left = 8
            Top = 44
            Width = 44
            Height = 13
            Caption = 'Te&mplate'
            FocusControl = ed_7z_Template
          end
          object Label29: TLabel
            Left = 8
            Top = 20
            Width = 38
            Height = 13
            Caption = 'Preview'
            FocusControl = ed_7z_Preview
          end
          object lbl_7zInfo_Template: TLabel
            Left = 128
            Top = 68
            Width = 108
            Height = 13
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object ed_7z_Template: TEdit
            Left = 128
            Top = 41
            Width = 334
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            TabOrder = 0
            Text = '$C $T $N -mx=5 $P $W $L'
            OnChange = ed_7z_TemplateChange
          end
          object ed_7z_Preview: TEdit
            Left = 128
            Top = 18
            Width = 416
            Height = 18
            Anchors = [akLeft, akTop, akRight]
            AutoSize = False
            BevelKind = bkFlat
            BorderStyle = bsNone
            Color = clSilver
            Ctl3D = True
            ParentCtl3D = False
            ReadOnly = True
            TabOrder = 2
          end
          object bt_7z_Default: TButton
            Left = 473
            Top = 41
            Width = 71
            Height = 21
            Anchors = [akTop, akRight]
            Caption = 'Default'
            TabOrder = 1
            OnClick = bt_7z_DefaultClick
          end
        end
        object gbx_7z_Variables: TGroupBox
          Left = 0
          Top = 109
          Width = 558
          Height = 188
          Align = alTop
          Caption = 'Variables'
          TabOrder = 1
          DesignSize = (
            558
            188)
          object Label18: TLabel
            Left = 8
            Top = 44
            Width = 85
            Height = 13
            Caption = '$T - &Archive Type'
            FocusControl = cmbx_7z_ArchiveType
          end
          object Label19: TLabel
            Left = 8
            Top = 92
            Width = 68
            Height = 13
            Caption = '$P - &Password'
            FocusControl = ed_7z_Password
          end
          object Label5: TLabel
            Left = 8
            Top = 20
            Width = 69
            Height = 13
            Caption = '$C - &Command'
            FocusControl = ed_7z_Command
          end
          object Label27: TLabel
            Left = 8
            Top = 68
            Width = 90
            Height = 13
            Caption = '$N - Archive Name'
          end
          object Label28: TLabel
            Left = 144
            Top = 68
            Width = 194
            Height = 13
            Caption = 'Destination.Directory + Destination.Name'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label30: TLabel
            Left = 8
            Top = 116
            Width = 111
            Height = 13
            Caption = '$W - Working Directory'
          end
          object Label31: TLabel
            Left = 144
            Top = 116
            Width = 175
            Height = 13
            Caption = '-w<Preferences.TemporaryDirectory>'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object lbl_7zInfo_Command: TLabel
            Left = 219
            Top = 21
            Width = 108
            Height = 13
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label32: TLabel
            Left = 8
            Top = 140
            Width = 50
            Height = 13
            Caption = '$L - Listfile'
          end
          object Label34: TLabel
            Left = 144
            Top = 140
            Width = 238
            Height = 13
            Caption = '@<Destination.Directory + Destination.Name>~.txt'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label2: TLabel
            Left = 144
            Top = 156
            Width = 249
            Height = 13
            Caption = 'Required. Instructs 7-Zip which files to use.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object lbl_7zInfo_ArchiveType: TLabel
            Left = 219
            Top = 41
            Width = 326
            Height = 25
            Anchors = [akLeft, akTop, akRight]
            AutoSize = False
            Caption = 'AUTO_TRANSLATED'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            Layout = tlCenter
            WordWrap = True
          end
          object cmbx_7z_ArchiveType: TComboBox
            Left = 144
            Top = 42
            Width = 65
            Height = 21
            ItemHeight = 13
            TabOrder = 1
            OnChange = cmbx_7z_ArchiveTypeChange
            Items.Strings = (
              '7z'
              'zip'
              'tar')
          end
          object ed_7z_Password: TEdit
            Left = 144
            Top = 89
            Width = 201
            Height = 21
            AutoSize = False
            PasswordChar = '*'
            TabOrder = 2
            OnChange = ed_7z_TemplateChange
          end
          object ed_7z_Command: TEdit
            Left = 144
            Top = 18
            Width = 25
            Height = 21
            MaxLength = 1
            TabOrder = 0
            Text = 'a'
            OnChange = ed_7z_TemplateChange
          end
        end
        object pnl_Descr_act7Zip: TPanel
          Left = 0
          Top = 0
          Width = 558
          Height = 20
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 2
          object Label24: TLabel
            Left = 8
            Top = 3
            Width = 453
            Height = 13
            Caption = 
              'Use 7-Zip to create a backup.  You should set the path to 7-Zip ' +
              'on the Preferences window first.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
        end
      end
    end
    object tab_File: TTabSheet
      Caption = 'File &Operation'
      ImageIndex = -1
      object pnl_Descr_actFile: TPanel
        Left = 0
        Top = 0
        Width = 562
        Height = 20
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
        object Label7: TLabel
          Left = 8
          Top = 3
          Width = 253
          Height = 13
          Caption = 'Settings for the File Operations Copy, Move or Delete.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 20
        Width = 562
        Height = 141
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 1
        DesignSize = (
          562
          141)
        object lbl_FileInfo_Overwrite: TLabel
          Left = 26
          Top = 24
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbl_FileInfo_UseRecycleBin: TLabel
          Left = 26
          Top = 87
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbl_File_PathInformation: TLabel
          Left = 8
          Top = 117
          Width = 77
          Height = 13
          Caption = '&Path Information'
          FocusControl = cmbx_File_PathInformation
        end
        object lbl_FileInfo_PathInformation: TLabel
          Left = 240
          Top = 112
          Width = 305
          Height = 25
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
        object cb_File_Overwrite: TCheckBox
          Left = 8
          Top = 8
          Width = 543
          Height = 17
          Anchors = [akLeft, akTop, akRight]
          Caption = 'O&verwrite Existing Files'
          Checked = True
          State = cbChecked
          TabOrder = 0
          OnClick = SetProfileModified
        end
        object cb_File_UseRecycleBin: TCheckBox
          Left = 8
          Top = 70
          Width = 543
          Height = 17
          Anchors = [akLeft, akTop, akRight]
          Caption = 'Use &Recycle Bin If Possible'
          Checked = True
          State = cbChecked
          TabOrder = 2
          OnClick = SetProfileModified
        end
        object cb_File_OverwriteReadOnly: TCheckBox
          Left = 26
          Top = 40
          Width = 523
          Height = 17
          Anchors = [akLeft, akTop, akRight]
          Caption = '&Also Overwrite Existing Read Only Files'
          TabOrder = 1
          OnClick = SetProfileModified
        end
        object cmbx_File_PathInformation: TComboBox
          Left = 112
          Top = 114
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 3
          OnSelect = SetProfileModified
        end
      end
    end
  end
  object pnl_Descr_ActSets: TPanel
    Left = 0
    Top = 0
    Width = 570
    Height = 20
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 1
    object Label14: TLabel
      Left = 8
      Top = 3
      Width = 231
      Height = 13
      Caption = 'Specify the settings for the selected Action Type.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
end
