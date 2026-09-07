class CharacterSheet extends MovieClip
{
   var BaseInfo_mc;
   var CategoryTitle;
   var CharDetails;
   var DebugText_mc;
   var FactionsHolder;
   var FactionsMask;
   var InfoHolder;
   var LevelMeter;
   var MenuHeader;
   var MenuLeftSide_mc;
   var MiscHolder;
   var PlayerInfoInstance;
   var SkillDetails;
   var SkillsHolder;
   var SkillsMask;
   var StatsHolder;
   var StatsMask;
   var _acceptButton;
   var _acceptControls;
   var _cancelButton;
   var _cancelControls;
   var _defaultControls;
   var _deleteControls;
   var _kinectControls;
   var _platform;
   var actionGamepadKey;
   var actionKey;
   var bGamepad;
   var bOpeningMenu;
   var bShowDetails;
   var bUpdated;
   var changeTitleButton;
   var changeTitleText;
   var debugTextField;
   var detailsGamepadKey;
   var detailsKey;
   var factionsListHeight;
   var healthMeter;
   var iCurrentFactionIndex;
   var iCurrentInfoIndex;
   var iCurrentPage;
   var iCurrentSkillIndex;
   var magickaMeter;
   var navLeftGamepadKey;
   var navLeftKey;
   var navRightGamepadKey;
   var navRightKey;
   var showDetailButton;
   var showDetailText;
   var skillsListHeight;
   var skillsMenuButton;
   var skillsMenuText;
   var staminaMeter;
   static var SKYUI_RELEASE_IDX = 16;
   static var SKYUI_VERSION_MAJOR = 4;
   static var SKYUI_VERSION_MINOR = 1;
   static var SKYUI_VERSION_STRING = CharacterSheet.SKYUI_VERSION_MAJOR + "." + CharacterSheet.SKYUI_VERSION_MINOR;
   static var INFO_IDX = 0;
   static var SKILLS_IDX = 1;
   static var FACTIONS_IDX = 2;
   static var STATS_IDX = 3;
   var scrollAmount = 40;
   var Categories = new Array();
   var Pages = new Array();
   var aInfoClipsContainer = new Array();
   var aSkillClipsContainer = new Array();
   var aFactionClipsContainer = new Array();
   function CharacterSheet()
   {
      super();
      gfx.managers.FocusHandler.instance.setFocus(this,0);
      Mouse.addListener(this);
      this.debugTextField = this.DebugText_mc.debugTextField;
      this.InfoHolder = this.MenuLeftSide_mc.InfoHolder_mc;
      this.SkillDetails = this.MenuLeftSide_mc.SkillDetails_mc;
      this.CharDetails = this.MenuLeftSide_mc.CharDetails_mc;
      this.SkillsHolder = this.MenuLeftSide_mc.SkillsHolder_mc;
      this.StatsHolder = this.MenuLeftSide_mc.StatsHolder_mc;
      this.FactionsHolder = this.MenuLeftSide_mc.FactionsHolder_mc;
      this.SkillsMask = this.MenuLeftSide_mc.SkillsMask_mc;
      this.FactionsMask = this.MenuLeftSide_mc.FactionsMask_mc;
      this.StatsMask = this.MenuLeftSide_mc.StatsMask_mc;
      this.CategoryTitle = this.MenuLeftSide_mc.CategoryTitle_mc;
      this.LevelMeter = this.BaseInfo_mc.LevelMeter_mc;
      this.MenuHeader = this.MenuLeftSide_mc.MenuHeader_mc;
      this.PlayerInfoInstance = this.BaseInfo_mc.PlayerInfoInstance_mc.PlayerInfoCardInstance;
      this.MiscHolder = this.BaseInfo_mc.MiscHolder_mc;
      this.magickaMeter = this.PlayerInfoInstance.MagickaMeterInstance.MagickaMeter_mc;
      this.healthMeter = this.PlayerInfoInstance.HealthMeterInstance.HealthMeter_mc;
      this.staminaMeter = this.PlayerInfoInstance.StaminaMeterInstance.StaminaMeter_mc;
      this.InfoHolder.name.title.text = "$NAME";
      this.InfoHolder.name.highlight._visible = false;
      this.InfoHolder.name.index = 0;
      this.InfoHolder.race.title.text = "$RACE";
      this.InfoHolder.race.highlight._visible = false;
      this.InfoHolder.race.index = 1;
      this.InfoHolder.constellation.title.text = "$CONSTELLATION";
      this.InfoHolder.constellation.highlight._visible = false;
      this.InfoHolder.constellation.index = 2;
      this.InfoHolder.attrClass.title.text = "$CLASS";
      this.InfoHolder.attrClass.highlight._visible = false;
      this.InfoHolder.attrClass.index = 3;
      this.InfoHolder.attrTrait.title.text = "$TRAIT";
      this.InfoHolder.attrTrait.highlight._visible = false;
      this.InfoHolder.attrTrait.index = 4;
      this.MenuHeader.playerTitle.text = "";
      this.SkillDetails._visible = false;
      this.CharDetails._visible = false;
      this.showDetailText.text = "$DETAIL_BUTTON_SHOW";
      this.showDetailText.textAutoSize = "shrink";
      this.skillsMenuText.text = "$OPEN_SKILLS_MENU";
      this.skillsMenuText.textAutoSize = "shrink";
      this.skillsMenuText._visible = false;
      this.skillsMenuButton._visible = false;
      this.changeTitleText.text = "$CHANGE_PLAYER_TITLE";
      this.changeTitleText.textAutoSize = "shrink";
      this.changeTitleText._visible = false;
      this.changeTitleButton._visible = false;
      this.SkillDetails.description.textAutoSize = "shrink";
      this.SkillDetails.description.verticalAlign = "center";
      this.CharDetails.description.textAutoSize = "shrink";
      this.CharDetails.description.verticalAlign = "center";
      this.SkillsHolder.setMask(this.SkillsMask);
      this.SkillsMask._visible = false;
      this.FactionsHolder.setMask(this.FactionsMask);
      this.FactionsMask._visible = false;
      this.StatsHolder.setMask(this.StatsMask);
      this.StatsMask._visible = false;
      this.iCurrentPage = CharacterSheet.INFO_IDX;
      this.iCurrentInfoIndex = -1;
      this.iCurrentSkillIndex = -1;
      this.iCurrentFactionIndex = -1;
      this._platform = 0;
      this.bUpdated = false;
      this.bOpeningMenu = false;
      this.bShowDetails = false;
   }
   function onLoad()
   {
      this.Pages.push({page:this.InfoHolder,index:CharacterSheet.INFO_IDX,text:"$CHARACTER"});
      this.Pages.push({page:this.SkillsHolder,index:CharacterSheet.SKILLS_IDX,text:"$SKILLS"});
      this.Pages.push({page:this.FactionsHolder,index:CharacterSheet.FACTIONS_IDX,text:"$FACTIONS"});
      this.Pages.push({page:this.StatsHolder,index:CharacterSheet.STATS_IDX,text:"$STATS"});
      this.Pages.sortOn("index");
      this.CategoryTitle.category1.textField.text = this.Pages[CharacterSheet.INFO_IDX].text;
      this.CategoryTitle.category1.textField.textAutoSize = "shrink";
      this.CategoryTitle.category1.index = CharacterSheet.INFO_IDX;
      this.CategoryTitle.category1.mask._alpha = 0;
      this.CategoryTitle.category1.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onCategoryHover(this._parent);
      };
      this.CategoryTitle.category1.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onCategoryRollOut(this._parent);
      };
      this.CategoryTitle.category1.mask.onMouseDown = function()
      {
         if(Mouse.getTopMostEntity() == this)
         {
            this._parent._parent._parent._parent.onCategoryClick(this._parent);
         }
      };
      this.CategoryTitle.category2.textField.text = this.Pages[CharacterSheet.SKILLS_IDX].text;
      this.CategoryTitle.category2.textField.textAutoSize = "shrink";
      this.CategoryTitle.category2.index = CharacterSheet.SKILLS_IDX;
      this.CategoryTitle.category2.mask._alpha = 0;
      this.CategoryTitle.category2.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onCategoryHover(this._parent);
      };
      this.CategoryTitle.category2.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onCategoryRollOut(this._parent);
      };
      this.CategoryTitle.category2.mask.onMouseDown = function()
      {
         if(Mouse.getTopMostEntity() == this)
         {
            this._parent._parent._parent._parent.onCategoryClick(this._parent);
         }
      };
      this.CategoryTitle.category3.textField.text = this.Pages[CharacterSheet.FACTIONS_IDX].text;
      this.CategoryTitle.category3.textField.textAutoSize = "shrink";
      this.CategoryTitle.category3.index = CharacterSheet.FACTIONS_IDX;
      this.CategoryTitle.category3.mask._alpha = 0;
      this.CategoryTitle.category3.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onCategoryHover(this._parent);
      };
      this.CategoryTitle.category3.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onCategoryRollOut(this._parent);
      };
      this.CategoryTitle.category3.mask.onMouseDown = function()
      {
         if(Mouse.getTopMostEntity() == this)
         {
            this._parent._parent._parent._parent.onCategoryClick(this._parent);
         }
      };
      this.CategoryTitle.category4.textField.text = this.Pages[CharacterSheet.STATS_IDX].text;
      this.CategoryTitle.category4.textField.textAutoSize = "shrink";
      this.CategoryTitle.category4.index = CharacterSheet.STATS_IDX;
      this.CategoryTitle.category4.mask._alpha = 0;
      this.CategoryTitle.category4.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onCategoryHover(this._parent);
      };
      this.CategoryTitle.category4.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onCategoryRollOut(this._parent);
      };
      this.CategoryTitle.category4.mask.onMouseDown = function()
      {
         if(Mouse.getTopMostEntity() == this)
         {
            this._parent._parent._parent._parent.onCategoryClick(this._parent);
         }
      };
      this.Categories.push(this.CategoryTitle.category1);
      this.Categories.push(this.CategoryTitle.category2);
      this.Categories.push(this.CategoryTitle.category3);
      this.Categories.push(this.CategoryTitle.category4);
      var _loc2_ = 0;
      while(_loc2_ < this.Pages.length)
      {
         if(_loc2_ == this.iCurrentPage)
         {
            this.Pages[_loc2_].page._visible = true;
            this.Categories[_loc2_].selector._visible = true;
            this.Categories[_loc2_].textField.textColor = 16777215;
            this.Categories[_loc2_].textField._alpha = 100;
         }
         else
         {
            this.Pages[_loc2_].page._visible = false;
            this.Categories[_loc2_].selector._visible = false;
            this.Categories[_loc2_].textField.textColor = 12369084;
            this.Categories[_loc2_].textField._alpha = 45;
         }
         _loc2_ += 1;
      }
      this.aInfoClipsContainer.push(this.InfoHolder.name);
      this.aInfoClipsContainer.push(this.InfoHolder.race);
      this.aInfoClipsContainer.push(this.InfoHolder.constellation);
      this.aInfoClipsContainer.push(this.InfoHolder.attrClass);
      this.aInfoClipsContainer.push(this.InfoHolder.attrTrait);
      this.SetPlatform(this._platform,false);
      gfx.io.GameDelegate.call("PlaySound",["UIJournalOpen"]);
   }
   function handleInput(details, pathToFocus)
   {
      var _loc4_ = false;
      var _loc5_;
      var _loc6_;
      var _loc7_;
      if(Shared.GlobalFunc.IsKeyPressed(details))
      {
         if(details.navEquivalent == gfx.ui.NavigationCode.TAB || details.navEquivalent == gfx.ui.NavigationCode.GAMEPAD_B)
         {
            this.CloseMenu();
            _loc4_ = true;
         }
         else if(details.code == this.navLeftGamepadKey || details.code == this.navLeftKey)
         {
            if(this.iCurrentPage == 0)
            {
               _loc5_ = this.Pages.length - 1;
            }
            else
            {
               _loc5_ = this.iCurrentPage - 1;
            }
            this.ChangePage(_loc5_);
            _loc4_ = true;
         }
         else if(details.code == this.navRightGamepadKey || details.code == this.navRightKey)
         {
            if(this.iCurrentPage >= this.Pages.length - 1)
            {
               _loc5_ = 0;
            }
            else
            {
               _loc5_ = this.iCurrentPage + 1;
            }
            this.ChangePage(_loc5_);
            _loc4_ = true;
         }
         else if(details.code == this.actionKey || details.code == this.actionGamepadKey)
         {
            if(this.skillsMenuButton._visible)
            {
               _loc6_ = this;
               _loc7_ = setTimeout(function()
               {
                  gfx.io.GameDelegate.call("OpenSkillsMenu",[]);
               }
               ,300);
               skyui.util.Tween.LinearTween(this,"_alpha",this._alpha,0,0.25);
            }
            else if(this.changeTitleButton._visible)
            {
               this.ChangePlayerTitle();
            }
            _loc4_ = true;
         }
         else if(details.navEquivalent == gfx.ui.NavigationCode.UP)
         {
            if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
            {
               if(this.iCurrentSkillIndex > 0)
               {
                  this.onSkillHover(this.aSkillClipsContainer[this.iCurrentSkillIndex - 1]);
                  if(this.aSkillClipsContainer[this.iCurrentSkillIndex]._y + this.SkillsHolder._y < -220)
                  {
                     this.SkillsHolder._y = -220 - this.aSkillClipsContainer[this.iCurrentSkillIndex]._y;
                  }
               }
            }
            else if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX)
            {
               if(this.iCurrentFactionIndex > 0)
               {
                  this.onFactionHover(this.aFactionClipsContainer[this.iCurrentFactionIndex - 1]);
                  if(this.aFactionClipsContainer[this.iCurrentFactionIndex]._y + this.FactionsHolder._y < -204)
                  {
                     this.FactionsHolder._y = -204 - this.aFactionClipsContainer[this.iCurrentFactionIndex]._y;
                  }
               }
            }
            else if(this.iCurrentPage == CharacterSheet.STATS_IDX)
            {
               this.StatsHolder._y += 45;
               if(this.StatsHolder._y > 0)
               {
                  this.StatsHolder._y = 0;
               }
            }
            _loc4_ = true;
         }
         else if(details.navEquivalent == gfx.ui.NavigationCode.DOWN)
         {
            if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
            {
               if(this.iCurrentSkillIndex < this.aSkillClipsContainer.length - 1)
               {
                  this.onSkillHover(this.aSkillClipsContainer[this.iCurrentSkillIndex + 1]);
                  if(this.aSkillClipsContainer[this.iCurrentSkillIndex]._y + this.SkillsHolder._y > 185)
                  {
                     this.SkillsHolder._y = 185 - this.aSkillClipsContainer[this.iCurrentSkillIndex]._y;
                  }
                  if(this.bShowDetails && this.iCurrentSkillIndex > -1)
                  {
                     this.SkillDetails._visible = true;
                  }
               }
            }
            else if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX)
            {
               if(this.iCurrentFactionIndex < this.aFactionClipsContainer.length - 1)
               {
                  this.onFactionHover(this.aFactionClipsContainer[this.iCurrentFactionIndex + 1]);
                  if(this.aFactionClipsContainer[this.iCurrentFactionIndex]._y + this.FactionsHolder._y > 175)
                  {
                     this.FactionsHolder._y = 175 - this.aFactionClipsContainer[this.iCurrentFactionIndex]._y;
                  }
                  this.changeTitleText._visible = true;
                  this.changeTitleButton._visible = true;
               }
            }
            else if(this.iCurrentPage == CharacterSheet.STATS_IDX)
            {
               this.StatsHolder._y -= 45;
               if(this.StatsHolder._y < this.StatsMask._y + this.StatsMask._height - this.StatsHolder._height)
               {
                  this.StatsHolder._y = this.StatsMask._y + this.StatsMask._height - this.StatsHolder._height;
               }
            }
            else if(this.iCurrentPage == CharacterSheet.INFO_IDX)
            {
               if(this.iCurrentInfoIndex < 4)
               {
                  this.onInfoHover(this.aInfoClipsContainer[this.iCurrentInfoIndex + 1]);
               }
               if(this.bShowDetails && this.iCurrentInfoIndex > 0)
               {
                  this.CharDetails._visible = true;
               }
            }
            _loc4_ = true;
         }
         else if(details.code == this.detailsKey || details.code == this.detailsGamepadKey)
         {
            this.bShowDetails = !this.bShowDetails;
            if(this.bShowDetails == true)
            {
               this.showDetailText.text = "$DETAIL_BUTTON_HIDE";
               gfx.io.GameDelegate.call("PlaySound",["UIMenuBladeOpenSD"]);
            }
            else
            {
               gfx.io.GameDelegate.call("PlaySound",["UIMenuBladeCloseSD"]);
               this.showDetailText.text = "$DETAIL_BUTTON_SHOW";
            }
            if(this.iCurrentSkillIndex > -1 && this.iCurrentPage == CharacterSheet.SKILLS_IDX)
            {
               if(this.SkillDetails._visible == false)
               {
                  this.SetSkillDetails(this.aSkillClipsContainer[this.iCurrentSkillIndex]);
                  this.SkillDetails._visible = true;
               }
               else
               {
                  this.SkillDetails._visible = false;
               }
            }
            else if(this.iCurrentInfoIndex > 0 && this.iCurrentPage == CharacterSheet.INFO_IDX)
            {
               if(this.CharDetails._visible == false)
               {
                  this.SetCharDetails(this.aInfoClipsContainer[this.iCurrentInfoIndex]);
                  this.CharDetails._visible = true;
               }
               else
               {
                  this.CharDetails._visible = false;
               }
            }
            _loc4_ = true;
         }
      }
      return _loc4_;
   }
   function CloseMenu(moveLeft)
   {
      gfx.io.GameDelegate.call("CloseMenu",[]);
   }
   function onMouseWheel(delta)
   {
      var _loc3_ = {_x:this._xmouse,_y:this._ymouse};
      var _loc4_ = 0.1;
      var _loc5_;
      if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
      {
         if(_loc3_._x >= this.SkillsMask._x && _loc3_._x <= this.SkillsMask._x + this.SkillsMask._width && _loc3_._y >= this.SkillsMask._y && _loc3_._y <= this.SkillsMask._y + this.SkillsMask._height)
         {
            _loc5_ = this.SkillsMask._y + this.SkillsMask._height - this.skillsListHeight + 11;
            if(Math.abs(this.SkillsHolder._y - (this.SkillsMask._y - 39)) < _loc4_ && delta > 0 || this.skillsListHeight - this.SkillsMask._height < _loc4_ && delta < 0)
            {
               return undefined;
            }
            this.SkillsHolder._y += delta * this.scrollAmount;
            if(this.SkillsHolder._y > this.SkillsMask._y - 39)
            {
               this.SkillsHolder._y = this.SkillsMask._y - 39;
            }
            if(this.SkillsHolder._y < _loc5_)
            {
               this.SkillsHolder._y = _loc5_;
            }
            return undefined;
         }
      }
      else if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX)
      {
         if(_loc3_._x >= this.FactionsMask._x && _loc3_._x <= this.FactionsMask._x + this.FactionsMask._width && _loc3_._y >= this.FactionsMask._y && _loc3_._y <= this.FactionsMask._y + this.FactionsMask._height)
         {
            _loc5_ = this.FactionsMask._y + this.FactionsMask._height - this.factionsListHeight + 63;
            if(Math.abs(this.FactionsHolder._y - (this.FactionsMask._y - 26)) < _loc4_ && delta > 0 || this.factionsListHeight - this.FactionsMask._height < _loc4_ && delta < 0)
            {
               return undefined;
            }
            this.FactionsHolder._y += delta * this.scrollAmount;
            if(this.FactionsHolder._y > this.FactionsMask._y - 26)
            {
               this.FactionsHolder._y = this.FactionsMask._y - 26;
            }
            if(this.FactionsHolder._y < _loc5_)
            {
               this.FactionsHolder._y = _loc5_;
            }
            return undefined;
         }
      }
      else if(this.iCurrentPage == CharacterSheet.STATS_IDX)
      {
         if(_loc3_._x >= this.StatsMask._x && _loc3_._x <= this.StatsMask._x + this.StatsMask._width && _loc3_._y >= this.StatsMask._y && _loc3_._y <= this.StatsMask._y + this.StatsMask._height)
         {
            _loc5_ = this.StatsMask._y + this.StatsMask._height - this.StatsHolder._height;
            if(Math.abs(this.StatsHolder._y - this.StatsMask._y) < _loc4_ && delta > 0 || this.StatsHolder._y - _loc5_ < _loc4_ && delta < 0)
            {
               return undefined;
            }
            this.StatsHolder._y += delta * this.scrollAmount;
            if(this.StatsHolder._y > this.StatsMask._y)
            {
               this.StatsHolder._y = this.StatsMask._y;
            }
            if(this.StatsHolder._y < _loc5_)
            {
               this.StatsHolder._y = _loc5_;
            }
            return undefined;
         }
      }
      return undefined;
   }
   function onCategoryHover(category)
   {
      if(category.index == this.iCurrentPage)
      {
         return undefined;
      }
      category.textField.textColor = 16777215;
      category.textField._alpha = 100;
   }
   function onCategoryRollOut(category)
   {
      if(category.index == this.iCurrentPage)
      {
         return undefined;
      }
      category.textField.textColor = 12369084;
      category.textField._alpha = 45;
   }
   function onCategoryClick(category)
   {
      if(category.index == this.iCurrentPage)
      {
         return undefined;
      }
      this.ChangePage(category.index);
   }
   function ChangePage(pageNum)
   {
      this.Pages[this.iCurrentPage].page._visible = false;
      this.Categories[this.iCurrentPage].selector._visible = false;
      this.Categories[this.iCurrentPage].textField.textColor = 12369084;
      this.Categories[this.iCurrentPage].textField._alpha = 45;
      this.iCurrentPage = pageNum;
      if(this.bShowDetails)
      {
         switch(this.iCurrentPage)
         {
            case CharacterSheet.INFO_IDX:
               if(this.iCurrentInfoIndex > 0)
               {
                  this.CharDetails._visible = true;
               }
               else
               {
                  this.CharDetails._visible = false;
               }
               this.SkillDetails._visible = false;
               break;
            case CharacterSheet.SKILLS_IDX:
               this.CharDetails._visible = false;
               if(this.iCurrentSkillIndex > -1)
               {
                  this.SkillDetails._visible = true;
               }
               else
               {
                  this.SkillDetails._visible = false;
               }
               break;
            case CharacterSheet.FACTIONS_IDX:
               this.CharDetails._visible = false;
               this.SkillDetails._visible = false;
            case CharacterSheet.STATS_IDX:
               this.CharDetails._visible = false;
               this.SkillDetails._visible = false;
         }
      }
      else
      {
         this.CharDetails._visible = false;
         this.SkillDetails._visible = false;
      }
      if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
      {
         this.skillsMenuButton._visible = true;
         this.skillsMenuText._visible = true;
         this.skillsMenuText.textAutoSize = "shrink";
         this.changeTitleButton._visible = false;
         this.changeTitleText._visible = false;
      }
      else if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX && this.iCurrentFactionIndex > -1)
      {
         this.changeTitleButton._visible = true;
         this.changeTitleText._visible = true;
         this.changeTitleText.textAutoSize = "shrink";
         this.skillsMenuButton._visible = false;
         this.skillsMenuText._visible = false;
      }
      else
      {
         this.skillsMenuButton._visible = false;
         this.skillsMenuText._visible = false;
         this.changeTitleButton._visible = false;
         this.changeTitleText._visible = false;
      }
      this.Pages[this.iCurrentPage].page._visible = true;
      this.Categories[this.iCurrentPage].selector._visible = true;
      this.Categories[this.iCurrentPage].textField.textColor = 16777215;
      this.Categories[this.iCurrentPage].textField._alpha = 100;
      gfx.io.GameDelegate.call("PlaySound",["UIMenuPrevNext"]);
   }
   function SetGenericData(playerName, race, level, xpProgress, datetime, constellation, raceDescription, constellationDescription, condition, lowerCase)
   {
      if(!lowerCase)
      {
         this.MenuHeader.CharacterName._visible = true;
         this.MenuHeader.CharacterNameMedium._visible = false;
         this.MenuHeader.CharacterName.text = playerName.toUpperCase();
      }
      else
      {
         this.MenuHeader.CharacterName._visible = false;
         this.MenuHeader.CharacterNameMedium._visible = true;
         this.MenuHeader.CharacterNameMedium.text = playerName;
      }
      this.MenuHeader.CharacterName.autoSize = "left";
      this.MenuHeader.playerTitle._x = this.MenuHeader.CharacterName._x + this.MenuHeader.CharacterName._width + 30;
      this.LevelMeter.LevelNumberLabel.text = level;
      this.LevelMeter.LevelNumberLabel.textAutoSize = "shrink";
      this.LevelMeter.LevelProgressBar.gotoAndStop(140 - xpProgress);
      this.InfoHolder.name.info.text = playerName;
      this.InfoHolder.name.mask._alpha = 0;
      this.InfoHolder.name.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onInfoHover(this._parent);
      };
      this.InfoHolder.name.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onInfoRollOut(this._parent);
      };
      this.InfoHolder.race.info.text = race;
      this.InfoHolder.race.descTitle = race;
      this.InfoHolder.race.description = raceDescription;
      this.InfoHolder.race.spec = "";
      this.InfoHolder.race.condition = condition;
      this.InfoHolder.race.mask._alpha = 0;
      this.InfoHolder.race.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onInfoHover(this._parent);
      };
      this.InfoHolder.race.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onInfoRollOut(this._parent);
      };
      this.InfoHolder.constellation.info.text = constellation;
      this.InfoHolder.constellation.descTitle = constellation;
      this.InfoHolder.constellation.description = constellationDescription;
      this.InfoHolder.constellation.spec = "";
      this.InfoHolder.constellation.mask._alpha = 0;
      this.InfoHolder.constellation.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onInfoHover(this._parent);
      };
      this.InfoHolder.constellation.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onInfoRollOut(this._parent);
      };
      this.MenuHeader.playerTitleRank._x = this.MenuHeader.CharacterName._x + this.MenuHeader.CharacterName.textWidth + 20;
      this.MenuHeader.playerTitleConnector._x = this.MenuHeader.playerTitleRank._x + this.MenuHeader.playerTitleRank._width;
      this.MenuHeader.playerTitleFaction._x = this.MenuHeader.playerTitleConnector._x + this.MenuHeader.playerTitleConnector._width;
   }
   function SetCharDetails(infoClip)
   {
      this.CharDetails.title.text = infoClip.descTitle;
      this.CharDetails.description.text = infoClip.description;
      var _loc3_ = infoClip.condition;
      if(_loc3_ == "" || _loc3_ == undefined)
      {
         this.CharDetails.playerCondition._visible = false;
      }
      else
      {
         this.CharDetails.playerCondition.gotoAndStop(_loc3_);
         this.CharDetails.playerCondition._visible = true;
      }
      var _loc4_ = infoClip.spec;
      if(_loc4_ == "")
      {
         this.CharDetails.specialization.text = "";
         this.CharDetails.specTitle._visible = false;
         return undefined;
      }
      this.CharDetails.specialization.text = _loc4_;
      this.CharDetails.specTitle._visible = true;
      if(_loc4_ == "$Magic")
      {
         this.CharDetails.specialization.textColor = 4948182;
      }
      else if(_loc4_ == "$Combat")
      {
         this.CharDetails.specialization.textColor = 14043979;
      }
      else
      {
         this.CharDetails.specialization.textColor = 4970080;
      }
   }
   function SetSkillDetails(skillClip)
   {
      this.SkillDetails.title.text = skillClip.name;
      this.SkillDetails.description.text = skillClip.description;
      this.SkillDetails.description.textAutoSize = "shrink";
      this.SkillDetails.level.text = skillClip.lvlText;
      this.SkillDetails.xpBar.gotoAndStop(skillClip.xpFrame);
      this.SkillDetails.skillIcon.gotoAndStop(skillClip.key);
   }
   function SetAttributesMeters(health, maxHealth, temporaryHealth, magicka, maxMagicka, temporaryMagicka, stamina, maxStamina, temporaryStamina)
   {
      this.SetMeter(this.healthMeter,this.PlayerInfoInstance.healthValue,health,maxHealth,temporaryHealth);
      this.SetMeter(this.magickaMeter,this.PlayerInfoInstance.magicValue,magicka,maxMagicka,temporaryMagicka);
      this.SetMeter(this.staminaMeter,this.PlayerInfoInstance.enduranceValue,stamina,maxStamina,temporaryStamina);
   }
   function SetMeter(meter, textArea, currentValue, maxValue, modifier)
   {
      var _loc6_ = 100 * (Math.max(0,Math.min(currentValue,maxValue)) / maxValue);
      meter.gotoAndStop(200 - 2 * _loc6_);
      textArea.text = Math.floor(currentValue) + "/" + Math.floor(maxValue);
      if(modifier > 0)
      {
         textArea.textColor = 1871638;
      }
      else if(modifier < 0)
      {
         textArea.textColor = 16449536;
      }
   }
   function SetSkills(skillsData, attributedClass, classSpecialization, classDescription, traitName, traitDescription)
   {
      this.alphabetSort(skillsData,"skillName");
      var _loc8_ = 0;
      var _loc9_;
      var _loc10_;
      var _loc11_;
      while(_loc8_ < skillsData.length)
      {
         _loc9_ = skillsData[_loc8_];
         _loc10_ = 45 * _loc8_;
         _loc11_ = this.SkillsHolder.attachMovie("skillSelector","skill" + _loc8_.toString(),this.SkillsHolder.getNextHighestDepth(),{_x:0,_y:_loc10_});
         _loc11_.textField.text = _loc9_.skillName;
         _loc11_.name = _loc9_.skillName;
         _loc11_.description = _loc9_.description;
         _loc11_.lvlText = _loc9_.level;
         _loc11_.level.text = _loc9_.level;
         _loc11_.key = _loc9_.key;
         _loc11_.xpFrame = _loc9_.xpFrame;
         _loc11_.highlight._visible = false;
         _loc11_.mask._alpha = 0;
         _loc11_.mask.onRollOver = function()
         {
            this._parent._parent._parent._parent.onSkillHover(this._parent);
         };
         _loc11_.mask.onRollOut = function()
         {
            this._parent._parent._parent._parent.onSkillRollOut(this._parent);
         };
         _loc11_.index = this.aSkillClipsContainer.length;
         this.aSkillClipsContainer.push(_loc11_);
         _loc8_ += 1;
      }
      if(traitName == "" && traitDescription == "")
      {
         this.InfoHolder.attrTrait._visible = false;
         this.InfoHolder.attrTraitIcon._visible = false;
      }
      else
      {
         this.InfoHolder.attrTrait._visible = true;
         this.InfoHolder.attrTraitIcon._visible = true;
         this.InfoHolder.attrTrait.info.text = traitName;
         this.InfoHolder.attrTrait.descTitle = traitName;
         this.InfoHolder.attrTrait.description = traitDescription;
         this.InfoHolder.attrTrait.spec = "";
         this.InfoHolder.attrTrait.mask._alpha = 0;
         this.InfoHolder.attrTrait.mask.onRollOver = function()
         {
            this._parent._parent._parent._parent.onInfoHover(this._parent);
         };
         this.InfoHolder.attrTrait.mask.onRollOut = function()
         {
            this._parent._parent._parent._parent.onInfoRollOut(this._parent);
         };
      }
      this.InfoHolder.attrClass.info.text = attributedClass;
      this.InfoHolder.attrClass.descTitle = attributedClass;
      this.InfoHolder.attrClass.description = classDescription;
      this.InfoHolder.attrClass.spec = classSpecialization;
      this.InfoHolder.attrClass.mask._alpha = 0;
      this.InfoHolder.attrClass.mask.onRollOver = function()
      {
         this._parent._parent._parent._parent.onInfoHover(this._parent);
      };
      this.InfoHolder.attrClass.mask.onRollOut = function()
      {
         this._parent._parent._parent._parent.onInfoRollOut(this._parent);
      };
      this.skillsListHeight = (skillsData.length + 1) * 45 - 5;
   }
   function onSkillHover(skillSelector)
   {
      if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
      {
         this.onSkillRollOut(this.aSkillClipsContainer[this.iCurrentSkillIndex]);
         this.iCurrentSkillIndex = skillSelector.index;
         skillSelector.highlight._visible = true;
         this.SetSkillDetails(skillSelector);
         if(this.bShowDetails)
         {
            this.SkillDetails._visible = true;
         }
      }
   }
   function onSkillRollOut(skillSelector)
   {
      if(this.iCurrentPage == CharacterSheet.SKILLS_IDX)
      {
         skillSelector.highlight._visible = false;
      }
   }
   function onInfoHover(infoHolder)
   {
      if(this.iCurrentPage == CharacterSheet.INFO_IDX)
      {
         this.onInfoRollOut(this.aInfoClipsContainer[this.iCurrentInfoIndex]);
         this.iCurrentInfoIndex = infoHolder.index;
         infoHolder.highlight._visible = true;
         this.SetCharDetails(infoHolder);
         if(this.bShowDetails && this.iCurrentInfoIndex > 0)
         {
            this.CharDetails._visible = true;
         }
      }
   }
   function onInfoRollOut(infoHolder)
   {
      if(this.iCurrentPage == CharacterSheet.INFO_IDX)
      {
         infoHolder.highlight._visible = false;
         this.CharDetails._visible = false;
      }
   }
   function onFactionHover(factionHolder)
   {
      if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX)
      {
         this.onFactionRollOut(this.aFactionClipsContainer[this.iCurrentFactionIndex]);
         this.iCurrentFactionIndex = factionHolder.index;
         factionHolder.highlight._visible = true;
      }
      this.changeTitleText._visible = true;
      this.changeTitleButton._visible = true;
   }
   function onFactionRollOut(factionHolder)
   {
      if(this.iCurrentPage == CharacterSheet.FACTIONS_IDX)
      {
         factionHolder.highlight._visible = false;
      }
      this.changeTitleText._visible = false;
      this.changeTitleButton._visible = false;
   }
   function SetFactions(factionsData)
   {
      for(var _loc3_ in this.FactionsHolder)
      {
         this.FactionsHolder[_loc3_].removeMovieClip();
      }
      var _loc4_;
      var _loc3_;
      var _loc5_;
      var _loc6_;
      if(factionsData.length == 0)
      {
         _loc4_ = this.FactionsHolder.attachMovie("factionSelector","faction" + _loc3_.toString(),this.FactionsHolder.getNextHighestDepth(),{_x:0,_y:offset});
         _loc4_.textField.text = "$NO_FACTION";
         _loc4_.rank.text = "";
         _loc4_.icon._visible = false;
         _loc4_.highlight._visible = false;
         _loc4_.mask._visible = 0;
         _loc4_.separator._visible = false;
         this.MenuHeader.playerTitleRank._visible = false;
         this.MenuHeader.playerTitleConnector._visible = false;
         this.MenuHeader.playerTitleFaction._visible = false;
      }
      else
      {
         _loc3_ = 0;
         while(_loc3_ < factionsData.length)
         {
            _loc6_ = 75 * _loc3_;
            _loc5_ = factionsData[_loc3_];
            _loc4_ = this.FactionsHolder.attachMovie("factionSelector","faction" + _loc3_.toString(),this.FactionsHolder.getNextHighestDepth(),{_x:0,_y:_loc6_});
            _loc4_.textField.text = _loc5_.factionName;
            _loc4_.textField.textAutoSize = "shrink";
            _loc4_.factionName = _loc5_.factionName;
            _loc4_.rank.text = _loc5_.rank;
            _loc4_.rank.textAutoSize = "shrink";
            _loc4_.rank = _loc5_.rank;
            _loc4_.rankOnly = _loc5_.rankDisplayOnly;
            _loc4_.icon.gotoAndStop(_loc5_.id);
            _loc4_.highlight._visible = false;
            _loc4_.mask._alpha = 0;
            _loc4_.mask.onRollOver = function()
            {
               this._parent._parent._parent._parent.onFactionHover(this._parent);
            };
            _loc4_.mask.onRollOut = function()
            {
               this._parent._parent._parent._parent.onFactionRollOut(this._parent);
            };
            _loc4_.index = this.aFactionClipsContainer.length;
            this.aFactionClipsContainer.push(_loc4_);
            _loc3_ += 1;
         }
         this.SetPlayerTitle(factionsData[0].rank,factionsData[0].factionName,factionsData[0].rankDisplayOnly);
      }
      this.factionsListHeight = (factionsData.length + 1) * 75 - 5;
   }
   function SetPlayerTitle(rank, faction, rankOnly)
   {
      if(rank == "" || faction == "")
      {
         if(this.aFactionClipsContainer[0].rank == undefined || this.aFactionClipsContainer[0].factionName == undefined)
         {
            this.MenuHeader.playerTitleRank._visible = false;
            this.MenuHeader.playerTitleConnector._visible = false;
            this.MenuHeader.playerTitleFaction._visible = false;
            return undefined;
         }
         this.MenuHeader.playerTitleRank.text = this.aFactionClipsContainer[0].rank;
         this.MenuHeader.playerTitleFaction.text = this.aFactionClipsContainer[0].factionName;
      }
      else
      {
         this.MenuHeader.playerTitleRank.text = rank;
         this.MenuHeader.playerTitleFaction.text = faction;
      }
      if(rankOnly)
      {
         this.MenuHeader.playerTitleConnector._visible = false;
         this.MenuHeader.playerTitleFaction._visible = false;
      }
      else
      {
         this.MenuHeader.playerTitleConnector._visible = true;
         this.MenuHeader.playerTitleFaction._visible = true;
         this.MenuHeader.playerTitleConnector.text = "$TITLE_CONNECTOR";
         this.MenuHeader.playerTitleRank.textAutoSize = "shrink";
         this.MenuHeader.playerTitleConnector.textAutoSize = "shrink";
         this.MenuHeader.playerTitleFaction.textAutoSize = "shrink";
      }
   }
   function ChangePlayerTitle()
   {
      gfx.io.GameDelegate.call("SaveFactionTitle",[this.aFactionClipsContainer[this.iCurrentFactionIndex].rank,this.aFactionClipsContainer[this.iCurrentFactionIndex].factionName,this.aFactionClipsContainer[this.iCurrentFactionIndex].rankOnly]);
      this.SetPlayerTitle(this.aFactionClipsContainer[this.iCurrentFactionIndex].rank,this.aFactionClipsContainer[this.iCurrentFactionIndex].factionName,this.aFactionClipsContainer[this.iCurrentFactionIndex].rankOnly);
   }
   function SetStats(healRate, magickaRate, staminaRate, speedMult, weaponSpeedMult, poisonResist, magicResist, fireResist, frostResist, shockResist, diseaseResist, rightHandPoiseDamage, leftHandPoiseDamage, poiseArmorResist, poiseMagicResist, armorDamageMitigation, critChance, critDamage, rightHandDamage, leftHandDamage)
   {
      this.StatsHolder.healRateValue.text = Math.floor(healRate);
      this.StatsHolder.magickaRateValue.text = Math.floor(magickaRate);
      this.StatsHolder.staminaRateValue.text = Math.floor(staminaRate);
      this.StatsHolder.speedValue.text = Math.floor(speedMult);
      this.StatsHolder.weaponSpeedValue.text = Math.floor(weaponSpeedMult);
      this.StatsHolder.poisonResistValue.text = Math.floor(poisonResist);
      this.StatsHolder.magicResistValue.text = Math.floor(magicResist);
      this.StatsHolder.fireResistValue.text = Math.floor(fireResist);
      this.StatsHolder.frostResistValue.text = Math.floor(frostResist);
      this.StatsHolder.shockResistValue.text = Math.floor(shockResist);
      this.StatsHolder.diseaseResistValue.text = Math.floor(diseaseResist);
      this.StatsHolder.rightHandPoiseDamageValue.text = Math.floor(rightHandPoiseDamage);
      this.StatsHolder.leftHandPoiseDamageValue.text = Math.floor(leftHandPoiseDamage);
      this.StatsHolder.poiseArmorResistValue.text = Math.floor(poiseArmorResist);
      this.StatsHolder.poiseMagicResistValue.text = Math.floor(poiseMagicResist);
      this.StatsHolder.armorDamageMitigationValue.text = Math.floor(armorDamageMitigation);
      this.StatsHolder.critChanceValue.text = Math.floor(critChance);
      this.StatsHolder.critDamageValue.text = Math.floor(critDamage);
      this.StatsHolder.rightHandDamageValue.text = Math.floor(rightHandDamage);
      this.StatsHolder.leftHandDamageValue.text = Math.floor(leftHandDamage);
   }
   function SetMiscData(gold, armor, carryWeight, maxCarryWeight, warmth)
   {
      this.MiscHolder.gold.text = gold;
      this.MiscHolder.armor.text = armor;
      this.MiscHolder.carryWeight.text = Math.ceil(carryWeight) + "/" + maxCarryWeight;
      if(warmth > -1)
      {
         this.MiscHolder.warmth._visible = true;
         this.MiscHolder.warmthIcon._visible = true;
         this.MiscHolder.warmth.text = warmth;
      }
      else
      {
         this.MiscHolder.warmth._visible = false;
         this.MiscHolder.warmthIcon._visible = false;
      }
   }
   function SetGamepad(gamepad, detailsKeyNum, actionKeyNum, navLeftKeyNum, navRightKeyNum, detailsGamepadKeyNum, actionGamepadKeyNum, navLeftGamepadKeyNum, navRightGamepadKeyNum)
   {
      this.detailsKey = DxScanToWindows.dxScanCodeToVK(detailsKeyNum);
      this.actionKey = DxScanToWindows.dxScanCodeToVK(actionKeyNum);
      this.navLeftKey = DxScanToWindows.dxScanCodeToVK(navLeftKeyNum);
      this.navRightKey = DxScanToWindows.dxScanCodeToVK(navRightKeyNum);
      this.detailsGamepadKey = DxScanToWindows.dxScanCodeToVK(detailsGamepadKeyNum);
      this.actionGamepadKey = DxScanToWindows.dxScanCodeToVK(actionGamepadKeyNum);
      this.navLeftGamepadKey = DxScanToWindows.dxScanCodeToVK(navLeftGamepadKeyNum);
      this.navRightGamepadKey = DxScanToWindows.dxScanCodeToVK(navRightGamepadKeyNum);
      this.bGamepad = gamepad;
      if(!this.bGamepad)
      {
         this.CategoryTitle.moveLeft.gotoAndStop(navLeftKeyNum);
         this.CategoryTitle.moveRight.gotoAndStop(navRightKeyNum);
         this.skillsMenuButton.gotoAndStop(actionKeyNum);
         this.changeTitleButton.gotoAndStop(actionKeyNum);
         this.showDetailButton.gotoAndStop(detailsKeyNum);
      }
      else
      {
         this.CategoryTitle.moveLeft.gotoAndStop(navLeftGamepadKeyNum);
         this.CategoryTitle.moveRight.gotoAndStop(navRightGamepadKeyNum);
         this.skillsMenuButton.gotoAndStop(actionGamepadKeyNum);
         this.changeTitleButton.gotoAndStop(actionGamepadKeyNum);
         this.showDetailButton.gotoAndStop(detailsGamepadKeyNum);
      }
   }
   function SetPlatform(a_platform, a_bPS3Switch)
   {
      var _loc4_ = this._platform != 0;
      if(a_platform == 0)
      {
         this._deleteControls = {keyCode:45};
         this._defaultControls = {keyCode:20};
         this._kinectControls = {keyCode:37};
         this._acceptControls = {keyCode:28};
         this._cancelControls = {keyCode:15};
      }
      else
      {
         this._deleteControls = {keyCode:278};
         this._defaultControls = {keyCode:279};
         this._kinectControls = {keyCode:275};
         this._acceptControls = {keyCode:276};
         this._cancelControls = {keyCode:277};
      }
      this._acceptButton.addEventListener("click",this,"onAcceptMousePress");
      this._cancelButton.addEventListener("click",this,"onCancelMousePress");
      this._platform = a_platform;
   }
   function alphabetSort(arr, prop)
   {
      arr.sort(function(a, b)
      {
         var _loc3_ = String(a[prop]);
         var _loc4_ = String(b[prop]);
         if(_loc3_.charAt(0) == "$")
         {
            _loc3_ = _loc3_.substr(1);
         }
         if(_loc4_.charAt(0) == "$")
         {
            _loc4_ = _loc4_.substr(1);
         }
         _loc3_ = _loc3_.toLowerCase();
         _loc4_ = _loc4_.toLowerCase();
         if(_loc3_ < _loc4_)
         {
            return -1;
         }
         if(_loc3_ > _loc4_)
         {
            return 1;
         }
         return 0;
      }
      );
      return arr;
   }
   function numericSort(arr, prop, descending)
   {
      arr.sort(function(a, b)
      {
         var _loc3_ = Number(a[prop]);
         var _loc4_ = Number(b[prop]);
         if(isNaN(_loc3_))
         {
            _loc3_ = 0;
         }
         if(isNaN(_loc4_))
         {
            _loc4_ = 0;
         }
         return !descending ? _loc3_ - _loc4_ : _loc4_ - _loc3_;
      }
      );
      return arr;
   }
   function SetWidescreen(widescreen)
   {
      if(!widescreen)
      {
         return undefined;
      }
      this.BaseInfo_mc._x += 160;
      this.MenuLeftSide_mc._x -= 110;
   }
   function debugLog(message)
   {
      var _loc3_;
      if(typeof message == "string")
      {
         this.debugTextField.text += message + "\n";
      }
      else if(typeof message == "object")
      {
         _loc3_ = "Object Details: ";
         for(var _loc4_ in message)
         {
            _loc3_ += _loc4_ + ": " + message[_loc4_] + "; ";
         }
         this.debugTextField.text += _loc3_ + "\n";
      }
   }
   function debugLogHandler(event)
   {
      this.debugLog(event.message);
   }
}
