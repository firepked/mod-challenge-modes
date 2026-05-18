-- Challenge Modes Addon for WoW 3.3.5a
-- Tracks active server-side challenge modes and colors bag items
-- Listens for CM_ACTIVE system messages OR manual /cm set command

-- ============================================================================
-- Constants
-- ============================================================================

local CHALLENGE_MODES = {
    [0] = { name = "Hardcore",        desc = "You only have ONE life" },
    [1] = { name = "Semi-Hardcore",   desc = "Lose gear and gold on death" },
    [2] = { name = "Self-Crafted",    desc = "Only items you crafted yourself" },
    [3] = { name = "Low-Quality",     desc = "Only white/grey quality items" },
    [4] = { name = "Slow XP",         desc = "0.5x experience rate" },
    [5] = { name = "Very Slow XP",    desc = "0.25x experience rate" },
    [6] = { name = "Quest XP Only",   desc = "XP from quests only" },
    [7] = { name = "Iron Man",        desc = "No grouping, enchants, pots" },
    [8] = { name = "HARDCORE_DEAD",   desc = "Character is permanently dead" },
}

local MODE_SELF_CRAFTED       = 2
local MODE_ITEM_QUALITY_LEVEL = 3
local MODE_IRON_MAN           = 7

-- Quality constants
local ITEM_QUALITY_POOR   = 0  -- grey
local ITEM_QUALITY_NORMAL = 1  -- white
local ITEM_QUALITY_UNCOMMON = 2  -- green
local ITEM_QUALITY_RARE     = 3  -- blue
local ITEM_QUALITY_EPIC     = 4  -- purple
local ITEM_QUALITY_LEGENDARY = 5 -- orange

-- ============================================================================
-- State
-- ============================================================================

local activeModes = {}              -- { [modeID] = true }
local modesKnown = false            -- true after CM_ACTIVE received OR manually set
local toggleOverlay = true
local bagCacheDirty = true

-- Scanning tooltip (hidden, for reading Self-Crafted creator)
local scanTooltip = CreateFrame("GameTooltip", "ChallengeModesScanTooltip", nil, "GameTooltipTemplate")
scanTooltip:SetOwner(WorldFrame, "ANCHOR_NONE")

-- ============================================================================
-- Core logic
-- ============================================================================

function IsModeActive(modeID)
    return activeModes[modeID] == true
end

function HasAnyModeActive()
    for k, v in pairs(activeModes) do
        if k ~= 8 and v then return true end
    end
    return false
end

function SetActiveModes(modeTable)
    activeModes = {}
    for _, id in ipairs(modeTable) do
        activeModes[id] = true
    end
    modesKnown = true
    bagCacheDirty = true
    UpdateDisplay()
    ScanAllBags()
end

function ParseActiveModes(data)
    local ids = {}
    for id in data:gmatch("(%d+)") do
        ids[tonumber(id)] = true
    end
    activeModes = ids
    modesKnown = true
    bagCacheDirty = true
    UpdateDisplay()
    ScanAllBags()
end

-- ============================================================================
-- Item quality
-- ============================================================================

--- Get item quality from an item ID. Returns 0-5 or nil.
function GetItemQuality(itemID)
    if not itemID then return nil end
    local _, _, quality = GetItemInfo(itemID)
    return quality
end

--- Get quality from container slot directly (fallback if GetItemInfo fails)
function GetSlotQuality(bag, slot)
    local texture, _, _, quality = GetContainerItemInfo(bag, slot)
    if quality then return quality end
    local itemID = GetContainerItemID(bag, slot)
    if itemID then return GetItemQuality(itemID) end
    return nil
end

-- ============================================================================
-- Item restriction checks
-- ============================================================================

--- Returns: isRestricted, reasonString
function CheckItemRestricted(bag, slot, itemID)
    if not HasAnyModeActive() then
        return false, nil
    end

    local quality = GetSlotQuality(bag, slot)

    -- Quality check: Low-Quality Gear + Iron Man
    if IsModeActive(MODE_ITEM_QUALITY_LEVEL) or IsModeActive(MODE_IRON_MAN) then
        if quality and quality > ITEM_QUALITY_NORMAL then
            return true, "Only white/grey quality items allowed"
        end
    end

    -- Self-Crafted check: only items YOU crafted
    if IsModeActive(MODE_SELF_CRAFTED) then
        if not IsItemSelfCrafted(bag, slot) then
            return true, "Not crafted by you (Self-Crafted active)"
        end
    end

    return false, nil
end

--- Scan tooltip for "Crafted by <player>" text
function IsItemSelfCrafted(bag, slot)
    scanTooltip:ClearLines()
    scanTooltip:SetBagItem(bag, slot)

    local playerName = UnitName("player")
    local tooltipName = scanTooltip:GetName()

    for i = 1, scanTooltip:NumLines() do
        local text = _G[tooltipName .. "TextLeft" .. i]
        if text then
            local line = text:GetText() or ""
            -- Check various localisation patterns
            if line:find("Crafted by") or line:find("Made by") or line:find("Created by")
               or line:find("Hergestellt von") or line:find("Erschaffen von") then
                return line:find(playerName, nil, true) ~= nil
            end
        end
    end
    return false
end

-- ============================================================================
-- Bag scanning
-- ============================================================================

local scanDebounce = nil

function ScanAllBagsDelayed()
    if scanDebounce then scanDebounce:Cancel() end
    scanDebounce = C_Timer.After(0.3, ScanAllBags)
end

function ScanAllBags()
    if not toggleOverlay then return end
    if not HasAnyModeActive() then
        -- Still reset colors to show we're in "no data" state
        ResetAllBagTints()
        return
    end

    for bag = 0, 4 do
        local slots = GetContainerNumSlots(bag)
        for slot = 1, slots do
            local itemID = GetContainerItemID(bag, slot)
            local button = _G["ContainerFrame" .. (bag + 1) .. "Item" .. slot]
            if itemID and button then
                local icon = _G[button:GetName() .. "Icon"]
                if icon then
                    local restricted, reason = CheckItemRestricted(bag, slot, itemID)
                    if restricted then
                        icon:SetVertexColor(1.0, 0.25, 0.25) -- red
                    else
                        icon:SetVertexColor(0.25, 1.0, 0.25) -- green
                    end
                end
            end
        end
    end
end

function ResetAllBagTints()
    for bag = 0, 4 do
        for slot = 1, GetContainerNumSlots(bag) do
            local button = _G["ContainerFrame" .. (bag + 1) .. "Item" .. slot]
            if button then
                local icon = _G[button:GetName() .. "Icon"]
                if icon then
                    icon:SetVertexColor(1.0, 1.0, 1.0)
                end
            end
        end
    end
end

-- ============================================================================
-- Info display frame
-- ============================================================================

local displayFrame = nil

function CreateDisplayFrame()
    local f = CreateFrame("Frame", "ChallengeModesDisplayFrame", UIParent)
    f:SetSize(200, 30)
    f:SetPoint("CENTER", UIParent, "CENTER", 0, -200)
    f:SetMovable(true)
    f:SetClampedToScreen(true)
    f:EnableMouse(true)
    f:RegisterForDrag("LeftButton")
    f:SetScript("OnDragStart", f.StartMoving)
    f:SetScript("OnDragStop", f.StopMovingOrSizing)

    f:SetBackdrop({
        bgFile = "Interface\\DialogFrame\\UI-DialogBox-Background",
        edgeFile = "Interface\\Tooltips\\UI-Tooltip-Border",
        tile = true, tileSize = 32, edgeSize = 16,
        insets = { left = 4, right = 4, top = 4, bottom = 4 },
    })
    f:SetBackdropColor(0, 0, 0, 0.85)
    f:SetBackdropBorderColor(0.3, 0.6, 1.0, 1.0)

    local title = f:CreateFontString(nil, "OVERLAY", "GameFontNormalSmall")
    title:SetText("Challenge Modes")
    title:SetPoint("TOP", f, "TOP", 0, -4)
    title:SetTextColor(0.3, 0.8, 1.0, 1.0)

    f:Hide()
    return f
end

function UpdateDisplay()
    if not displayFrame then
        displayFrame = CreateDisplayFrame()
    end

    local lines = {}
    for id, mode in pairs(CHALLENGE_MODES) do
        if IsModeActive(id) then
            tinsert(lines, mode.name)
        end
    end

    if #lines == 0 then
        displayFrame:Hide()
        return
    end

    -- Find or create content frame
    local content = displayFrame.content
    if not content then
        content = CreateFrame("Frame", nil, displayFrame)
        content:SetPoint("TOPLEFT", displayFrame, "TOPLEFT", 8, -20)
        content:SetPoint("BOTTOMRIGHT", displayFrame, "BOTTOMRIGHT", -8, 8)
        displayFrame.content = content
    end

    -- Destroy old children
    for _, child in pairs({ content:GetChildren() }) do
        child:Hide()
        child:SetParent(nil)
    end

    -- Build lines
    local y = -4
    for _, name in ipairs(lines) do
        local fs = content:CreateFontString(nil, "OVERLAY", "GameFontNormalSmall")
        fs:SetText("  |cff00ff00\u{25CF}|r " .. name)
        fs:SetPoint("TOPLEFT", content, "TOPLEFT", 0, y)
        y = y - 15
    end

    local h = math.max(28, 24 + #lines * 15)
    displayFrame:SetHeight(h)
    content:SetHeight(h)
    displayFrame:Show()
end

-- ============================================================================
-- Tooltip hook
-- ============================================================================

local orig_SetBagItem = GameTooltip.SetBagItem
function GameTooltip:SetBagItem(bag, slot, ...)
    orig_SetBagItem(self, bag, slot, ...)

    if not HasAnyModeActive() then return end

    local itemID = GetContainerItemID(bag, slot)
    if not itemID then return end

    local restricted, reason = CheckItemRestricted(bag, slot, itemID)
    if restricted then
        self:AddLine(" ")
        self:AddLine("|cffff3333\u{2716} " .. reason, 1.0, 0.3, 0.3)
        if reason:find("Self%-Crafted") then
            self:AddLine("|cff888888Craft this item yourself to equip it.", 0.53, 0.53, 0.53)
        elseif reason:find("quality") then
            self:AddLine("|cff888888Only white or grey items may be equipped.", 0.53, 0.53, 0.53)
        end
    elseif HasAnyModeActive() then
        self:AddLine(" ")
        self:AddLine("|cff33ff33\u{2714} Eligible for all active challenges", 0.3, 1.0, 0.3)
    end
end

-- ============================================================================
-- Slash command
-- ============================================================================

SLASH_CHALLENGEMODES1 = "/cm"
SLASH_CHALLENGEMODES2 = "/challengemodes"

SlashCmdList["CHALLENGEMODES"] = function(msg)
    msg = strtrim(msg or "")
    local cmd, args = msg:match("^(%S*)%s*(.-)$")
    cmd = strlower(cmd or "")
    args = strtrim(args or "")

    if cmd == "" then
        toggleOverlay = not toggleOverlay
        if toggleOverlay then
            ScanAllBags()
            UpdateDisplay()
            print("|cff3399ffChallenge Modes|r overlay |cff33ff33ON|r")
        else
            ResetAllBagTints()
            if displayFrame then displayFrame:Hide() end
            print("|cff3399ffChallenge Modes|r overlay |cffff6666OFF|r")
        end

    elseif cmd == "set" then
        local ids = {}
        for num in args:gmatch("(%d+)") do
            tinsert(ids, tonumber(num))
        end
        if #ids == 0 then
            print("|cffff6666Usage:|r /cm set <modeID> [modeID ...]")
            print("  Mode IDs: 0=Hardcore 1=Semi-Hardcore 2=Self-Crafted 3=Low-Quality")
            print("            4=SlowXP 5=VerySlowXP 6=QuestXPOnly 7=IronMan 8=HCDead")
            return
        end
        SetActiveModes(ids)
        print("|cff3399ffChallenge Modes|r set manually:")
        for _, id in ipairs(ids) do
            if CHALLENGE_MODES[id] then
                print("  |cff00ff00\u{25CF}|r " .. CHALLENGE_MODES[id].name)
            end
        end
        -- Force a bag rescan immediately
        ScanAllBags()

    elseif cmd == "reset" or cmd == "rescan" then
        bagCacheDirty = true
        ScanAllBags()
        UpdateDisplay()
        print("|cff3399ffChallenge Modes|r rescanned.")

    elseif cmd == "clear" then
        activeModes = {}
        modesKnown = false
        ResetAllBagTints()
        if displayFrame then displayFrame:Hide() end
        print("|cff3399ffChallenge Modes|r cleared.")

    elseif cmd == "status" then
        if not modesKnown then
            print("|cff3399ffChallenge Modes:|r No mode data received yet.")
            print("  Use |cffffffff/cm set <ids>|r to set manually, or update the server mod.")
        elseif not HasAnyModeActive() then
            print("|cff3399ffChallenge Modes:|r No active challenges on this character.")
        else
            print("|cff3399ffChallenge Modes:|r Active:")
            for id, val in pairs(activeModes) do
                if val and CHALLENGE_MODES[id] then
                    print("  |cff00ff00\u{25CF}|r " .. CHALLENGE_MODES[id].name .. " - " .. CHALLENGE_MODES[id].desc)
                end
            end
        end

    else
        print("|cffff6666Unknown command:|r " .. cmd)
        print("  /cm                    - toggle overlay")
        print("  /cm set <id> [id ...] - manually set active modes")
        print("  /cm status             - show current state")
        print("  /cm clear             - clear all modes")
        print("  /cm reset             - force bag rescan")
    end
end

-- ============================================================================
-- Event handler
-- ============================================================================

local eventFrame = CreateFrame("Frame")
eventFrame:RegisterEvent("CHAT_MSG_SYSTEM")
eventFrame:RegisterEvent("BAG_UPDATE")
eventFrame:RegisterEvent("BANKFRAME_OPENED")
eventFrame:RegisterEvent("PLAYER_ENTERING_WORLD")

eventFrame:SetScript("OnEvent", function(self, event, ...)
    if event == "CHAT_MSG_SYSTEM" then
        local msg = ...
        if msg and msg:sub(1, 9) == "CM_ACTIVE" then
            ParseActiveModes(msg:sub(11))
        end
    elseif event == "BAG_UPDATE" or event == "BANKFRAME_OPENED" then
        if toggleOverlay then
            ScanAllBagsDelayed()
        end
    elseif event == "PLAYER_ENTERING_WORLD" then
        activeModes = {}
        modesKnown = false
        if displayFrame then displayFrame:Hide() end
        ResetAllBagTints()
    end
end)

-- ============================================================================
-- Auto-rescan when item tooltips are shown (catches newly looted items)
-- ============================================================================
local hookFrame = CreateFrame("Frame")
hookFrame:RegisterEvent("ITEM_PUSH")
hookFrame:SetScript("OnEvent", function()
    if toggleOverlay and HasAnyModeActive() then
        ScanAllBagsDelayed()
    end
end)

-- ============================================================================
-- Startup message
-- ============================================================================

local function OnAddonLoaded()
    print("|cff3399ffChallenge Modes|r addon loaded. Type |cffffffff/cm|r for help.")
    print("  No server mod update yet? Use |cffffffff/cm set 0 2|r to test Hardcore + Self-Crafted.")
end

local loadedFrame = CreateFrame("Frame")
loadedFrame:RegisterEvent("ADDON_LOADED")
loadedFrame:SetScript("OnEvent", function(self, event, addonName)
    if addonName == "ChallengeModes" then
        OnAddonLoaded()
        self:UnregisterEvent("ADDON_LOADED")
    end
end)
