object FrameSource: TFrameSource
  Left = 0
  Top = 0
  Width = 771
  Height = 379
  ParentShowHint = False
  ShowHint = True
  TabOrder = 0
  object Splitter: TSplitter
    Left = 242
    Top = 20
    Width = 6
    Height = 359
    Cursor = crHSplit
    AutoSnap = False
    MinSize = 144
    ResizeStyle = rsUpdate
  end
  object gbx_Files: TGroupBox
    Left = 248
    Top = 20
    Width = 523
    Height = 359
    Align = alClient
    Caption = 'Fi&les'
    TabOrder = 1
    object lv_Files: TListView
      Left = 2
      Top = 37
      Width = 519
      Height = 302
      Align = alClient
      Columns = <
        item
          Caption = 'Name'
          MinWidth = 50
          Width = 110
        end
        item
          Alignment = taRightJustify
          Caption = 'Size'
          MinWidth = 10
          Width = 60
        end
        item
          Caption = 'Attributes'
          MinWidth = 10
          Width = 42
        end
        item
          Alignment = taRightJustify
          Caption = 'Modified'
          MinWidth = 10
          Width = 112
        end
        item
          Alignment = taRightJustify
          Caption = 'Age'
        end
        item
          Caption = 'Filter'
          MinWidth = 20
          Width = 128
        end>
      HideSelection = False
      ReadOnly = True
      PopupMenu = pop_FilterSet
      SmallImages = DM.imgs_Filters
      TabOrder = 0
      ViewStyle = vsReport
      OnColumnClick = lv_FilesColumnClick
      OnCompare = lv_FilesCompare
      OnDblClick = lv_FilesDblClick
    end
    object pnl_FilesSummary: TPanel
      Left = 2
      Top = 339
      Width = 519
      Height = 18
      Align = alBottom
      BevelOuter = bvLowered
      TabOrder = 2
      object lbl_FilesSummary: TLabel
        Left = 8
        Top = 2
        Width = 80
        Height = 13
        Caption = 'lbl_FilesSummary'
      end
    end
    object pnl_FileToolBars: TPanel
      Left = 2
      Top = 15
      Width = 519
      Height = 22
      Align = alTop
      BevelOuter = bvNone
      Ctl3D = True
      ParentCtl3D = False
      TabOrder = 1
      object tbr_FilterIcons: TToolBar
        Left = 100
        Top = 0
        Width = 419
        Height = 22
        Align = alClient
        AutoSize = True
        ButtonWidth = 46
        EdgeBorders = []
        EdgeInner = esNone
        EdgeOuter = esNone
        Flat = True
        Images = DM.imgs_Filters
        List = True
        ShowCaptions = True
        TabOrder = 1
        Wrapable = False
        object tb_FileSort: TToolButton
          Left = 0
          Top = 0
          AllowAllUp = True
          AutoSize = True
          Caption = 'Sort'
          DropdownMenu = pop_FileSort
          Style = tbsDropDown
          OnClick = tb_FileSortClick
        end
      end
      object tbr_FilterSets: TToolBar
        Left = 0
        Top = 0
        Width = 100
        Height = 22
        Align = alLeft
        AutoSize = True
        ButtonWidth = 71
        EdgeBorders = []
        EdgeInner = esNone
        EdgeOuter = esNone
        Flat = True
        Images = DM.imgs_Buttons
        List = True
        ShowCaptions = True
        TabOrder = 0
        Wrapable = False
        object tb_FilterSetFunctions: TToolButton
          Left = 0
          Top = 0
          Hint = '[Right Mousebutton]'
          AllowAllUp = True
          AutoSize = True
          Caption = 'Filter&Set'
          DropdownMenu = pop_FilterSet
          ImageIndex = 2
          Style = tbsDropDown
          OnClick = tb_FilterSetFunctionsClick
        end
        object ToolButton3: TToolButton
          Left = 84
          Top = 0
          Width = 16
          Caption = 'ToolButton1'
          Style = tbsDivider
        end
      end
    end
  end
  object gbx_Dirs: TGroupBox
    Left = 0
    Top = 20
    Width = 242
    Height = 359
    Align = alLeft
    Caption = '&Directories'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    object tv_Dirs: TTreeView
      Left = 2
      Top = 37
      Width = 238
      Height = 320
      Align = alClient
      ChangeDelay = 50
      Color = 16580607
      HideSelection = False
      Indent = 19
      MultiSelectStyle = [msControlSelect, msVisibleOnly]
      PopupMenu = pop_Dirs
      ReadOnly = True
      SortType = stText
      StateImages = DM.imgs_CheckBoxes
      TabOrder = 0
      ToolTips = False
      OnChange = tv_DirsChange
      OnCollapsing = tv_DirsCollapsing
      OnExpanding = tv_DirsExpanding
      OnKeyDown = tv_DirsKeyDown
      OnKeyPress = tv_DirsKeyPress
      OnMouseDown = tv_DirsMouseDown
    end
    object tbr_Directories: TToolBar
      Left = 2
      Top = 15
      Width = 238
      Height = 22
      AutoSize = True
      ButtonWidth = 73
      EdgeBorders = []
      Flat = True
      Images = DM.imgs_Buttons
      List = True
      ShowCaptions = True
      TabOrder = 1
      object tb_DirectoryAdd: TToolButton
        Left = 0
        Top = 0
        Hint = '[Insert]'
        AutoSize = True
        Caption = '&Add'
        ImageIndex = 0
        OnClick = tb_DirectoryAddClick
      end
      object tb_DirectoryRemove: TToolButton
        Left = 50
        Top = 0
        Hint = '[Delete]'
        AutoSize = True
        Caption = '&Remove'
        ImageIndex = 1
        OnClick = tb_DirectoryRemoveClick
      end
      object ToolButton1: TToolButton
        Left = 121
        Top = 0
        Width = 8
        Caption = 'ToolButton1'
        ImageIndex = 3
        Style = tbsSeparator
      end
      object tb_DirectoriesMoreFunctions: TToolButton
        Left = 129
        Top = 0
        Hint = '[Right Mousebutton]'
        AutoSize = True
        Caption = '&More'
        DropdownMenu = pop_Dirs
        ImageIndex = 2
        Style = tbsDropDown
        OnClick = tb_DirectoriesMoreFunctionsClick
      end
    end
  end
  object pnl_Descr_Source: TPanel
    Left = 0
    Top = 0
    Width = 771
    Height = 20
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 2
    object Label15: TLabel
      Left = 8
      Top = 3
      Width = 361
      Height = 13
      Caption = 
        'Select the directories to include. Unselected directories are al' +
        'ways excluded.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object ActionList: TActionList
    Left = 88
    Top = 88
    object act_RefreshDirList: TAction
      Caption = 'Refresh &Directory List'
      ShortCut = 116
      OnExecute = act_RefreshDirListExecute
    end
    object act_RefreshFileList: TAction
      Caption = 'Refresh &File List'
      ShortCut = 8308
      OnExecute = act_RefreshFileListExecute
    end
    object act_SelectAllChildren: TAction
      Category = 'Popup_Dirs'
      Caption = '&Select + All Children'
      ShortCut = 16416
      OnExecute = act_SelectAllChildrenExecute
    end
    object act_UnSelectAllChildren: TAction
      Category = 'Popup_Dirs'
      Caption = '&UnSelect + All Children'
      ShortCut = 24608
      OnExecute = act_UnSelectAllChildrenExecute
    end
    object act_CollapseAllChildren: TAction
      Category = 'Popup_Dirs'
      Caption = '&Collapse + All Children'
      OnExecute = act_CollapseAllChildrenExecute
    end
    object act_ExpandAllChildren: TAction
      Category = 'Popup_Dirs'
      Caption = '&Expand + All Children'
      OnExecute = act_ExpandAllChildrenExecute
    end
    object act_ScanTreeBelowDir: TAction
      Category = 'Popup_Dirs'
      Caption = 'Scan &Tree Below Directory'
      ShortCut = 16500
      OnExecute = act_ScanTreeBelowDirExecute
    end
  end
  object pop_Dirs: TPopupMenu
    OnPopup = pop_DirsPopup
    Left = 24
    Top = 88
    object mi_SelectAllChildren: TMenuItem
      Action = act_SelectAllChildren
    end
    object mi_UnSelectAllChildren: TMenuItem
      Action = act_UnSelectAllChildren
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object mi_CollapseAllChildren: TMenuItem
      Action = act_CollapseAllChildren
    end
    object mi_ExpandAllChildren: TMenuItem
      Action = act_ExpandAllChildren
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object mi_ScanTreeBelowDir: TMenuItem
      Action = act_ScanTreeBelowDir
    end
  end
  object pop_FilterSet: TPopupMenu
    Images = DM.imgs_Filters
    OnPopup = pop_FilterSetPopup
    Left = 272
    Top = 88
    object pmi_Exclude: TMenuItem
      Caption = '&Exclude'
      ImageIndex = 0
      object pmi_ThisFile: TMenuItem
        Caption = 'This &File'
        OnClick = pmi_ThisFileClick
      end
      object pmi_FiletypeAllDirs: TMenuItem
        Caption = 'Filetype In All &Directories'
        OnClick = pmi_FiletypeAllDirsClick
      end
      object pmi_FiletypeThisDir: TMenuItem
        Caption = 'Filetype In &This Directory Only'
        OnClick = pmi_FiletypeThisDirClick
      end
      object pmi_FiletypeDirAndSubdirs: TMenuItem
        Caption = 'Filetype In This Directory And &Below'
        OnClick = pmi_FiletypeDirAndSubdirsClick
      end
      object pmi_AllFilesThisDir: TMenuItem
        Caption = '&All Files In This Dir'
        OnClick = pmi_AllFilesThisDirClick
      end
    end
    object pmi_Include: TMenuItem
      Caption = '&Include (Always)'
      ImageIndex = 1
    end
    object pmi_NoCompression: TMenuItem
      Caption = 'No &Compression'
      ImageIndex = 2
    end
    object pmi_AlwaysShowEditor: TMenuItem
      Caption = '&Always Show Editor'
      Checked = True
      OnClick = pmi_AlwaysShowEditorClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object pmi_NewFilterSetBasedOnFile: TMenuItem
      Caption = '&New FilterSet'
      OnClick = pmi_NewFilterSetBasedOnFileClick
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object pmi_EditActiveFilterSet: TMenuItem
      Caption = 'E&dit Active FilterSet'
      OnClick = pmi_EditActiveFilterSetClick
    end
  end
  object pop_FileSort: TPopupMenu
    Images = DM.imgs_Filters
    OnPopup = pop_FileSortPopup
    Left = 344
    Top = 88
    object pmi_None: TMenuItem
      Caption = 'None'
      OnClick = tb_FileSortItemClick
    end
    object pmi_Excluded: TMenuItem
      Caption = 'Excluded'
      ImageIndex = 0
      OnClick = tb_FileSortItemClick
    end
    object pmi_Included: TMenuItem
      Caption = 'Included'
      ImageIndex = 1
      OnClick = tb_FileSortItemClick
    end
    object pmi_NotCompressed: TMenuItem
      Caption = 'Not Compressed'
      ImageIndex = 2
      OnClick = tb_FileSortItemClick
    end
  end
end
