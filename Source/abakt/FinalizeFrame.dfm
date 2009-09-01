object FrameFinalize: TFrameFinalize
  Left = 0
  Top = 0
  Width = 440
  Height = 303
  TabOrder = 0
  object ScrollBox_Finalize: TScrollBox
    Left = 0
    Top = 0
    Width = 440
    Height = 303
    VertScrollBar.Tracking = True
    Align = alClient
    BevelInner = bvNone
    BevelOuter = bvNone
    TabOrder = 0
    object Panel1: TPanel
      Left = 0
      Top = 139
      Width = 436
      Height = 40
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 2
      DesignSize = (
        436
        40)
      object lbl_TestArchiveInfo: TLabel
        Left = 26
        Top = 22
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
      object cb_TestArchive: TCheckBox
        Left = 8
        Top = 4
        Width = 417
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Te&st Archive'
        TabOrder = 0
        OnClick = SetProfileModified
      end
    end
    object Panel10: TPanel
      Left = 0
      Top = 179
      Width = 436
      Height = 40
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 1
      DesignSize = (
        436
        40)
      object lbl_RemoveEmptyDirsInfo: TLabel
        Left = 26
        Top = 22
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
      object cb_RemoveEmptyDirs: TCheckBox
        Left = 8
        Top = 4
        Width = 417
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Re&move Emptied Source Directories'
        TabOrder = 0
        OnClick = SetProfileModified
      end
    end
    object gbx_History: TGroupBox
      Left = 0
      Top = 20
      Width = 436
      Height = 67
      Align = alTop
      Caption = 'H&istory'
      TabOrder = 0
      object lbl_HistoryEnabled: TLabel
        Left = 8
        Top = 46
        Width = 87
        Height = 13
        Caption = 'lbl_HistoryEnabled'
      end
      object bt_History: TButton
        Left = 8
        Top = 16
        Width = 177
        Height = 25
        Caption = 'Manage History'
        TabOrder = 0
        OnClick = bt_HistoryClick
      end
    end
    object pnl_Descr_Finalize: TPanel
      Left = 0
      Top = 0
      Width = 436
      Height = 20
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 3
      object Label9: TLabel
        Left = 8
        Top = 3
        Width = 264
        Height = 13
        Caption = 'Things to do when an Action has finished without errors.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
    end
    object gbx_FileList: TGroupBox
      Left = 0
      Top = 87
      Width = 436
      Height = 52
      Align = alTop
      Caption = 'File&List'
      TabOrder = 4
      DesignSize = (
        436
        52)
      object lbl_FileList_Enabled_Info: TLabel
        Left = 26
        Top = 34
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
      object cb_FileList_Enabled: TCheckBox
        Left = 8
        Top = 16
        Width = 417
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Enabled'
        TabOrder = 0
        OnClick = SetProfileModified
      end
    end
  end
end
