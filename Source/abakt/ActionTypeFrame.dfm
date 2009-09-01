object FrameActionType: TFrameActionType
  Left = 0
  Top = 0
  Width = 580
  Height = 323
  AutoScroll = False
  TabOrder = 0
  object ScrollBox_ActionType: TScrollBox
    Left = 0
    Top = 0
    Width = 580
    Height = 323
    VertScrollBar.Tracking = True
    Align = alClient
    BevelInner = bvNone
    BevelOuter = bvNone
    TabOrder = 0
    object pnl_Descr_ActionType: TPanel
      Left = 0
      Top = 0
      Width = 576
      Height = 20
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 0
      object Label15: TLabel
        Left = 8
        Top = 3
        Width = 255
        Height = 13
        Caption = 'Select the Action Type and specify the Archive Mode.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
    end
    object gbx_ActionType: TGroupBox
      Left = 0
      Top = 20
      Width = 576
      Height = 70
      Align = alTop
      Caption = 'Action Type'
      TabOrder = 1
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 85
        Height = 13
        Caption = 'Create Archive'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 160
        Top = 16
        Width = 80
        Height = 13
        Caption = 'File Operation'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object rb_File_Copy: TRadioButton
        Left = 160
        Top = 32
        Width = 113
        Height = 17
        Caption = '&Copy'
        TabOrder = 2
        OnClick = SetModified
      end
      object rb_Zip: TRadioButton
        Left = 8
        Top = 32
        Width = 137
        Height = 17
        Caption = '&Zip'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = SetModified
      end
      object rb_7Zip: TRadioButton
        Left = 8
        Top = 48
        Width = 137
        Height = 17
        Caption = '&7-Zip'
        TabOrder = 1
        OnClick = SetModified
      end
      object rb_File_Delete: TRadioButton
        Left = 280
        Top = 32
        Width = 113
        Height = 17
        Caption = '&Delete'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = SetModified
      end
      object rb_File_Move: TRadioButton
        Left = 160
        Top = 48
        Width = 113
        Height = 17
        Caption = '&Move'
        TabOrder = 3
        OnClick = SetModified
      end
    end
    object gbx_ArchiveMode: TGroupBox
      Left = 0
      Top = 90
      Width = 576
      Height = 134
      Align = alTop
      Caption = '&Archive Mode'
      TabOrder = 2
      object Label16: TLabel
        Left = 120
        Top = 32
        Width = 229
        Height = 13
        Caption = 'Use all selected files (ignoring A). Don'#39't modify A.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label29: TLabel
        Left = 120
        Top = 48
        Width = 252
        Height = 13
        Caption = 'Use all selected files (ignoring A). Clear A after Action.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label32: TLabel
        Left = 120
        Top = 64
        Width = 149
        Height = 13
        Caption = 'Use A-files only. Don'#39't modify A.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label33: TLabel
        Left = 120
        Top = 80
        Width = 172
        Height = 13
        Caption = 'Use A-files only. Clear A after Action.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label34: TLabel
        Left = 120
        Top = 96
        Width = 170
        Height = 13
        Caption = 'Use non A-files only. Don'#39't modify A.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lbl_ArchiveModeInfo: TLabel
        Left = 8
        Top = 114
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
      object Label3: TLabel
        Left = 8
        Top = 16
        Width = 243
        Height = 13
        Caption = 'How to deal with the Archive attribute (A)?'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object rb_AMode_Default: TRadioButton
        Left = 8
        Top = 32
        Width = 105
        Height = 17
        Caption = 'Default'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = ModeModified
      end
      object rb_AMode_Full: TRadioButton
        Left = 8
        Top = 48
        Width = 105
        Height = 17
        Caption = 'Full'
        TabOrder = 1
        OnClick = ModeModified
      end
      object rb_AMode_Differential: TRadioButton
        Left = 8
        Top = 64
        Width = 105
        Height = 17
        Caption = 'Differential'
        TabOrder = 2
        OnClick = ModeModified
      end
      object rb_AMode_Incremental: TRadioButton
        Left = 8
        Top = 80
        Width = 105
        Height = 17
        Caption = 'Incremental'
        TabOrder = 3
        OnClick = ModeModified
      end
      object rb_AMode_Inverse: TRadioButton
        Left = 8
        Top = 96
        Width = 105
        Height = 17
        Caption = 'Inverse'
        TabOrder = 4
        OnClick = ModeModified
      end
    end
    object Panel2: TPanel
      Left = 0
      Top = 224
      Width = 576
      Height = 29
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 3
      DesignSize = (
        576
        29)
      object cb_IncludeEmptySourceDirectories: TCheckBox
        Left = 8
        Top = 6
        Width = 553
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = '&Include Empty Source Directories'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = SetModified
      end
    end
  end
end
