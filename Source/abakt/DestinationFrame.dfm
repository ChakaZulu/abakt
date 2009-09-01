object FrameDestination: TFrameDestination
  Left = 0
  Top = 0
  Width = 614
  Height = 352
  AutoScroll = False
  TabOrder = 0
  object ScrollBox_Dest: TScrollBox
    Left = 0
    Top = 0
    Width = 614
    Height = 352
    VertScrollBar.Tracking = True
    Align = alClient
    TabOrder = 0
    object gbx_Name: TGroupBox
      Left = 0
      Top = 91
      Width = 610
      Height = 129
      Align = alTop
      Caption = 'Name'
      TabOrder = 2
      DesignSize = (
        610
        129)
      object Label29: TLabel
        Left = 8
        Top = 42
        Width = 44
        Height = 13
        Caption = 'Te&mplate'
        FocusControl = ed_NameTemplate
      end
      object Label32: TLabel
        Left = 8
        Top = 17
        Width = 38
        Height = 13
        Caption = 'Preview'
      end
      object Label34: TLabel
        Left = 64
        Top = 70
        Width = 45
        Height = 13
        Alignment = taRightJustify
        Caption = 'Examples'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lbl_AppendExtension: TLabel
        Left = 26
        Top = 110
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
      object ed_NameTemplate: TEdit
        Left = 120
        Top = 39
        Width = 474
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        BevelEdges = [beLeft, beTop, beRight]
        TabOrder = 1
        OnChange = ed_NameTemplateChange
      end
      object ed_FormattedNamePreview: TEdit
        Left = 120
        Top = 15
        Width = 474
        Height = 18
        TabStop = False
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        BevelKind = bkFlat
        BorderStyle = bsNone
        Color = clSilver
        Ctl3D = True
        ParentCtl3D = False
        ReadOnly = True
        TabOrder = 0
      end
      object cb_AppendExtension: TCheckBox
        Left = 8
        Top = 92
        Width = 585
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Append Exten&sion'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = cb_AppendExtensionClick
      end
      object cmbx_NameTemplateExamples: TComboBox
        Left = 120
        Top = 66
        Width = 474
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 2
        OnSelect = cmbx_NameTemplateExamplesSelect
        Items.Strings = (
          'Choose...'
          '<Name>'
          '<Name>-MyText'
          '<Name>-BAK-<Date>'
          '<Name>-<Date>-<ArchiveMode>'
          '<Name>-<Date@Time>'
          'Backup of Profile <Name>, made on <Day, Date>')
      end
    end
    object pnl_Descr_Destination: TPanel
      Left = 0
      Top = 0
      Width = 610
      Height = 20
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 0
      object Label15: TLabel
        Left = 8
        Top = 3
        Width = 224
        Height = 13
        Caption = 'Specify the Destination directory and (file)name.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
    end
    object gbx_Directory: TGroupBox
      Left = 0
      Top = 20
      Width = 610
      Height = 71
      Align = alTop
      Caption = 'Directory'
      TabOrder = 1
      DesignSize = (
        610
        71)
      object bt_BrowseDir: TButton
        Left = 8
        Top = 19
        Width = 105
        Height = 22
        Caption = '&Browse'
        TabOrder = 0
        OnClick = bt_BrowseDirClick
      end
      object ed_Directory: TEdit
        Left = 120
        Top = 20
        Width = 474
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        BevelEdges = [beLeft, beTop, beRight]
        TabOrder = 1
        OnChange = ed_DirectoryChange
      end
      object cb_AutoCreateDir: TCheckBox
        Left = 8
        Top = 46
        Width = 585
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Automatically &Create Directory'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = SetProfileModified
      end
    end
  end
end
