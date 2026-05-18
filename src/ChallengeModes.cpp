/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "ChallengeModes.h"

// ============================================================================
// Mode info helpers
// ============================================================================

std::vector<ChallengeModeInfo> ChallengeModes::GetAllModeInfos()
{
    return {
        { SETTING_HARDCORE,           "Hardcore",           "You only have one life. Upon death you become a ghost permanently and can never be revived." },
        { SETTING_SEMI_HARDCORE,      "Semi-Hardcore",      "Upon death you lose all worn equipment and carried gold." },
        { SETTING_SELF_CRAFTED,       "Self-Crafted",       "You can only equip items you have crafted yourself." },
        { SETTING_ITEM_QUALITY_LEVEL, "Low-Quality Gear",   "You can only equip items of Normal or Poor quality." },
        { SETTING_SLOW_XP_GAIN,       "Slow XP",            "Experience gain is reduced to 50%." },
        { SETTING_VERY_SLOW_XP_GAIN,  "Very Slow XP",       "Experience gain is reduced to 25%. Includes Slow XP rewards." },
        { SETTING_QUEST_XP_ONLY,      "Quest XP Only",      "Experience can only be gained from quests." },
        { SETTING_IRON_MAN,           "Iron Man",           "Enforces the Iron Man challenge ruleset (no trading, no grouping, no enchants, etc.)." }
    };
}

std::string ChallengeModes::GetModeName(ChallengeModeSettings id)
{
    auto const& infos = GetAllModeInfos();
    for (auto const& info : infos)
    {
        if (info.id == id)
            return info.name;
    }
    return "Unknown";
}

std::string ChallengeModes::GetModeDescription(ChallengeModeSettings id)
{
    auto const& infos = GetAllModeInfos();
    for (auto const& info : infos)
    {
        if (info.id == id)
            return info.description;
    }
    return "";
}

// ============================================================================
// Singleton
// ============================================================================

ChallengeModes* ChallengeModes::instance()
{
    static ChallengeModes instance;
    return &instance;
}

// ============================================================================
// Config helpers
// ============================================================================

bool ChallengeModes::challengeEnabledForPlayer(ChallengeModeSettings setting, Player* player) const
{
    if (!enabled() || !challengeEnabled(setting))
    {
        return false;
    }
    return player->GetPlayerSetting("mod-challenge-modes", setting).value;
}

bool ChallengeModes::challengeEnabled(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return hardcoreEnable;
        case SETTING_SEMI_HARDCORE:
            return semiHardcoreEnable;
        case SETTING_SELF_CRAFTED:
            return selfCraftedEnable;
        case SETTING_ITEM_QUALITY_LEVEL:
            return itemQualityLevelEnable;
        case SETTING_SLOW_XP_GAIN:
            return slowXpGainEnable;
        case SETTING_VERY_SLOW_XP_GAIN:
            return verySlowXpGainEnable;
        case SETTING_QUEST_XP_ONLY:
            return questXpOnlyEnable;
        case SETTING_IRON_MAN:
            return ironManEnable;
        case HARDCORE_DEAD:
            break;
    }
    return false;
}

uint32 ChallengeModes::getDisableLevel(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return hardcoreDisableLevel;
        case SETTING_SEMI_HARDCORE:
            return semiHardcoreDisableLevel;
        case SETTING_SELF_CRAFTED:
            return selfCraftedDisableLevel;
        case SETTING_ITEM_QUALITY_LEVEL:
            return itemQualityLevelDisableLevel;
        case SETTING_SLOW_XP_GAIN:
            return slowXpGainDisableLevel;
        case SETTING_VERY_SLOW_XP_GAIN:
            return verySlowXpGainDisableLevel;
        case SETTING_QUEST_XP_ONLY:
            return questXpOnlyDisableLevel;
        case SETTING_IRON_MAN:
            return ironManDisableLevel;
        case HARDCORE_DEAD:
            break;
    }
    return 0;
}

float ChallengeModes::getXpBonusForChallenge(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return hardcoreXpBonus;
        case SETTING_SEMI_HARDCORE:
            return semiHardcoreXpBonus;
        case SETTING_SELF_CRAFTED:
            return selfCraftedXpBonus;
        case SETTING_ITEM_QUALITY_LEVEL:
            return itemQualityLevelXpBonus;
        case SETTING_SLOW_XP_GAIN:
            return slowXpGainBonus;
        case SETTING_VERY_SLOW_XP_GAIN:
            return verySlowXpGainBonus;
        case SETTING_QUEST_XP_ONLY:
            return questXpOnlyXpBonus;
        case SETTING_IRON_MAN:
            return ironManXpBonus;
        case HARDCORE_DEAD:
            break;
    }
    return 1.0f;
}

// ============================================================================
// Reward map getters
// ============================================================================

const std::unordered_map<uint8, uint32>* ChallengeModes::getTitleMapForChallenge(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return &hardcoreTitleRewards;
        case SETTING_SEMI_HARDCORE:
            return &semiHardcoreTitleRewards;
        case SETTING_SELF_CRAFTED:
            return &selfCraftedTitleRewards;
        case SETTING_ITEM_QUALITY_LEVEL:
            return &itemQualityLevelTitleRewards;
        case SETTING_SLOW_XP_GAIN:
            return &slowXpGainTitleRewards;
        case SETTING_VERY_SLOW_XP_GAIN:
            return &verySlowXpGainTitleRewards;
        case SETTING_QUEST_XP_ONLY:
            return &questXpOnlyTitleRewards;
        case SETTING_IRON_MAN:
            return &ironManTitleRewards;
        case HARDCORE_DEAD:
            break;
    }
    return {};
}

const std::unordered_map<uint8, uint32>* ChallengeModes::getTalentMapForChallenge(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return &hardcoreTalentRewards;
        case SETTING_SEMI_HARDCORE:
            return &semiHardcoreTalentRewards;
        case SETTING_SELF_CRAFTED:
            return &selfCraftedTalentRewards;
        case SETTING_ITEM_QUALITY_LEVEL:
            return &itemQualityLevelTalentRewards;
        case SETTING_SLOW_XP_GAIN:
            return &slowXpGainTalentRewards;
        case SETTING_VERY_SLOW_XP_GAIN:
            return &verySlowXpGainTalentRewards;
        case SETTING_QUEST_XP_ONLY:
            return &questXpOnlyTalentRewards;
        case SETTING_IRON_MAN:
            return &ironManTalentRewards;
        case HARDCORE_DEAD:
            break;
    }
    return {};
}

const std::unordered_map<uint8, uint32>* ChallengeModes::getItemMapForChallenge(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return &hardcoreItemRewards;
        case SETTING_SEMI_HARDCORE:
            return &semiHardcoreItemRewards;
        case SETTING_SELF_CRAFTED:
            return &selfCraftedItemRewards;
        case SETTING_ITEM_QUALITY_LEVEL:
            return &itemQualityLevelItemRewards;
        case SETTING_SLOW_XP_GAIN:
            return &slowXpGainItemRewards;
        case SETTING_VERY_SLOW_XP_GAIN:
            return &verySlowXpGainItemRewards;
        case SETTING_QUEST_XP_ONLY:
            return &questXpOnlyItemRewards;
        case SETTING_IRON_MAN:
            return &ironManItemRewards;
        case HARDCORE_DEAD:
            break;
    }
    return {};
}

uint32 ChallengeModes::getItemRewardAmount(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return hardcoreItemRewardAmount;
        case SETTING_SEMI_HARDCORE:
            return semiHardcoreItemRewardAmount;
        case SETTING_SELF_CRAFTED:
            return selfCraftedItemRewardAmount;
        case SETTING_ITEM_QUALITY_LEVEL:
            return itemQualityLevelItemRewardAmount;
        case SETTING_SLOW_XP_GAIN:
            return slowXpGainItemRewardAmount;
        case SETTING_VERY_SLOW_XP_GAIN:
            return verySlowXpGainItemRewardAmount;
        case SETTING_QUEST_XP_ONLY:
            return questXpOnlyItemRewardAmount;
        case SETTING_IRON_MAN:
            return ironManItemRewardAmount;
        case HARDCORE_DEAD:
            break;
    }
    return 0;
}

const std::unordered_map<uint8, uint32>* ChallengeModes::getAchievementMapForChallenge(ChallengeModeSettings setting) const
{
    switch (setting)
    {
        case SETTING_HARDCORE:
            return &hardcoreAchievementReward;
        case SETTING_SEMI_HARDCORE:
            return &semiHardcoreAchievementReward;
        case SETTING_SELF_CRAFTED:
            return &selfCraftedAchievementReward;
        case SETTING_ITEM_QUALITY_LEVEL:
            return &itemQualityLevelAchievementReward;
        case SETTING_SLOW_XP_GAIN:
            return &slowXpGainAchievementReward;
        case SETTING_VERY_SLOW_XP_GAIN:
            return &verySlowXpGainAchievementReward;
        case SETTING_QUEST_XP_ONLY:
            return &questXpOnlyAchievementReward;
        case SETTING_IRON_MAN:
            return &ironManAchievementReward;
        case HARDCORE_DEAD:
            break;
    }
    return {};
}

// ============================================================================
// World script — loads config
// ============================================================================

class ChallengeModes_WorldScript : public WorldScript
{
public:
    ChallengeModes_WorldScript()
        : WorldScript("ChallengeModes_WorldScript")
    {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        LoadConfig();
    }

private:
    static void LoadStringToMap(std::unordered_map<uint8, uint32>& mapToLoad, const std::string& configString)
    {
        std::string delimitedValue;
        std::stringstream configIdStream;
        configIdStream.str(configString);

        while (std::getline(configIdStream, delimitedValue, ','))
        {
            std::string pairOne, pairTwo;
            std::stringstream configPairStream(delimitedValue);
            configPairStream >> pairOne >> pairTwo;
            uint8 configLevel = static_cast<uint8>(atoi(pairOne.c_str()));
            uint32 rewardValue = static_cast<uint32>(atoi(pairTwo.c_str()));
            mapToLoad[configLevel] = rewardValue;
        }
    }

    static void LoadConfig()
    {
        sChallengeModes->challengesEnabled = sConfigMgr->GetOption<bool>("ChallengeModes.Enable", false);
        if (!sChallengeModes->enabled())
        {
            return;
        }

        for (auto& [confName, rewardMap] : sChallengeModes->rewardConfigMap)
        {
            rewardMap->clear();
            LoadStringToMap(*rewardMap, sConfigMgr->GetOption<std::string>(confName, ""));
        }

        sChallengeModes->hardcoreEnable          = sConfigMgr->GetOption<bool>("Hardcore.Enable", true);
        sChallengeModes->semiHardcoreEnable      = sConfigMgr->GetOption<bool>("SemiHardcore.Enable", true);
        sChallengeModes->selfCraftedEnable       = sConfigMgr->GetOption<bool>("SelfCrafted.Enable", true);
        sChallengeModes->itemQualityLevelEnable  = sConfigMgr->GetOption<bool>("ItemQualityLevel.Enable", true);
        sChallengeModes->slowXpGainEnable        = sConfigMgr->GetOption<bool>("SlowXpGain.Enable", true);
        sChallengeModes->verySlowXpGainEnable    = sConfigMgr->GetOption<bool>("VerySlowXpGain.Enable", true);
        sChallengeModes->questXpOnlyEnable       = sConfigMgr->GetOption<bool>("QuestXpOnly.Enable", true);
        sChallengeModes->ironManEnable           = sConfigMgr->GetOption<bool>("IronMan.Enable", true);

        sChallengeModes->hardcoreDisableLevel          = sConfigMgr->GetOption<uint32>("Hardcore.DisableLevel", 0);
        sChallengeModes->semiHardcoreDisableLevel      = sConfigMgr->GetOption<uint32>("SemiHardcore.DisableLevel", 0);
        sChallengeModes->selfCraftedDisableLevel       = sConfigMgr->GetOption<uint32>("SelfCrafted.DisableLevel", 0);
        sChallengeModes->itemQualityLevelDisableLevel  = sConfigMgr->GetOption<uint32>("ItemQualityLevel.DisableLevel", 0);
        sChallengeModes->slowXpGainDisableLevel        = sConfigMgr->GetOption<uint32>("SlowXpGain.DisableLevel", 0);
        sChallengeModes->verySlowXpGainDisableLevel    = sConfigMgr->GetOption<uint32>("VerySlowXpGain.DisableLevel", 0);
        sChallengeModes->questXpOnlyDisableLevel       = sConfigMgr->GetOption<uint32>("QuestXpOnly.DisableLevel", 0);
        sChallengeModes->ironManDisableLevel           = sConfigMgr->GetOption<uint32>("IronMan.DisableLevel", 0);

        sChallengeModes->hardcoreXpBonus         = sConfigMgr->GetOption<float>("Hardcore.XPMultiplier", 1.0f);
        sChallengeModes->semiHardcoreXpBonus     = sConfigMgr->GetOption<float>("SemiHardcore.XPMultiplier", 1.0f);
        sChallengeModes->selfCraftedXpBonus      = sConfigMgr->GetOption<float>("SelfCrafted.XPMultiplier", 1.0f);
        sChallengeModes->itemQualityLevelXpBonus = sConfigMgr->GetOption<float>("ItemQualityLevel.XPMultiplier", 1.0f);
        sChallengeModes->questXpOnlyXpBonus      = sConfigMgr->GetOption<float>("QuestXpOnly.XPMultiplier", 1.0f);
        sChallengeModes->slowXpGainBonus         = sConfigMgr->GetOption<float>("SlowXpGain.XPMultiplier", 0.50f);
        sChallengeModes->verySlowXpGainBonus     = sConfigMgr->GetOption<float>("VerySlowXpGain.XPMultiplier", 0.25f);
        sChallengeModes->ironManXpBonus          = sConfigMgr->GetOption<float>("IronMan.XPMultiplier", 1.0f);

        sChallengeModes->hardcoreItemRewardAmount         = sConfigMgr->GetOption<uint32>("Hardcore.ItemRewardAmount", 1);
        sChallengeModes->semiHardcoreItemRewardAmount     = sConfigMgr->GetOption<uint32>("SemiHardcore.ItemRewardAmount", 1);
        sChallengeModes->selfCraftedItemRewardAmount      = sConfigMgr->GetOption<uint32>("SelfCrafted.ItemRewardAmount", 1);
        sChallengeModes->itemQualityLevelItemRewardAmount = sConfigMgr->GetOption<uint32>("ItemQualityLevel.ItemRewardAmount", 1);
        sChallengeModes->slowXpGainItemRewardAmount       = sConfigMgr->GetOption<uint32>("SlowXpGain.ItemRewardAmount", 1);
        sChallengeModes->verySlowXpGainItemRewardAmount   = sConfigMgr->GetOption<uint32>("VerySlowXpGain.ItemRewardAmount", 1);
        sChallengeModes->questXpOnlyItemRewardAmount      = sConfigMgr->GetOption<uint32>("QuestXpOnly.ItemRewardAmount", 1);
        sChallengeModes->ironManItemRewardAmount          = sConfigMgr->GetOption<uint32>("IronMan.ItemRewardAmount", 1);

        LoadStringToMap(sChallengeModes->hardcoreAchievementReward, sConfigMgr->GetOption<std::string>("Hardcore.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->semiHardcoreAchievementReward, sConfigMgr->GetOption<std::string>("SemiHardcore.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->selfCraftedAchievementReward, sConfigMgr->GetOption<std::string>("SelfCrafted.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->itemQualityLevelAchievementReward, sConfigMgr->GetOption<std::string>("ItemQualityLevel.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->slowXpGainAchievementReward, sConfigMgr->GetOption<std::string>("SlowXpGain.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->verySlowXpGainAchievementReward, sConfigMgr->GetOption<std::string>("VerySlowXpGain.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->questXpOnlyAchievementReward, sConfigMgr->GetOption<std::string>("QuestXpOnly.AchievementReward", ""));
        LoadStringToMap(sChallengeModes->ironManAchievementReward, sConfigMgr->GetOption<std::string>("IronMan.AchievementReward", ""));
    }
};

// ============================================================================
// Base ChallengeMode (shared XP/level logic)
// ============================================================================

class ChallengeMode : public PlayerScript
{
public:
    explicit ChallengeMode(const char* scriptName, ChallengeModeSettings settingName)
        : PlayerScript(scriptName), _settingName(settingName)
    {}

    static bool mapContainsKey(const std::unordered_map<uint8, uint32>* mapToCheck, uint8 key)
    {
        return mapToCheck->find(key) != mapToCheck->end();
    }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* /*victim*/, uint8 /*xpSource*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(_settingName, player))
            return;

        amount *= sChallengeModes->getXpBonusForChallenge(_settingName);
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(_settingName, player))
            return;

        uint8 level = player->GetLevel();

        // Title reward
        if (auto const* titleMap = sChallengeModes->getTitleMapForChallenge(_settingName))
        {
            if (mapContainsKey(titleMap, level))
            {
                uint32 titleId = titleMap->at(level);
                if (auto const* titleInfo = sCharTitlesStore.LookupEntry(titleId))
                {
                    player->SetTitle(titleInfo);
                }
                else
                {
                    LOG_ERROR("mod-challenge-modes", "Invalid title ID {} for {}", titleId, _settingName);
                }
            }
        }

        // Talent reward
        if (auto const* talentMap = sChallengeModes->getTalentMapForChallenge(_settingName))
        {
            if (mapContainsKey(talentMap, level))
            {
                player->RewardExtraBonusTalentPoints(talentMap->at(level));
            }
        }

        // Achievement reward
        if (auto const* achievementMap = sChallengeModes->getAchievementMapForChallenge(_settingName))
        {
            if (mapContainsKey(achievementMap, level))
            {
                uint32 achievementId = achievementMap->at(level);
                if (auto const* achievementInfo = sAchievementStore.LookupEntry(achievementId))
                {
                    player->CompletedAchievement(achievementInfo);
                }
                else
                {
                    LOG_ERROR("mod-challenge-modes", "Invalid Achievement ID {} for {}", achievementId, _settingName);
                }
            }
        }

        // Item reward
        if (auto const* itemMap = sChallengeModes->getItemMapForChallenge(_settingName))
        {
            if (mapContainsKey(itemMap, level))
            {
                uint32 itemEntry = itemMap->at(level);
                uint32 itemAmount = sChallengeModes->getItemRewardAmount(_settingName);
                player->SendItemRetrievalMail({ { itemEntry, itemAmount } });
            }
        }

        // Auto-disable at cap
        if (sChallengeModes->getDisableLevel(_settingName) && sChallengeModes->getDisableLevel(_settingName) <= level)
        {
            player->UpdatePlayerSetting("mod-challenge-modes", _settingName, 0);
        }
    }

private:
    ChallengeModeSettings _settingName;
};

// ============================================================================
// Hardcore mode
// ============================================================================

class ChallengeMode_Hardcore : public ChallengeMode
{
public:
    ChallengeMode_Hardcore() : ChallengeMode("ChallengeMode_Hardcore", SETTING_HARDCORE) {}

    void OnPlayerLogin(Player* player) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_HARDCORE, player) || !sChallengeModes->challengeEnabledForPlayer(HARDCORE_DEAD, player))
            return;

        // Character is permanently dead — let them see the ghost but prevent play
        ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Your Hardcore character has died and cannot be revived.|r");
    }

    void OnPlayerReleasedGhost(Player* player) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_HARDCORE, player))
            return;

        // First death: mark as permanently dead
        if (!sChallengeModes->challengeEnabledForPlayer(HARDCORE_DEAD, player))
        {
            player->UpdatePlayerSetting("mod-challenge-modes", HARDCORE_DEAD, 1);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Your Hardcore character has died! You are now a ghost permanently.|r");
        }
    }

    void OnPlayerPVPKill(Player* /*killer*/, Player* killed) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_HARDCORE, killed))
            return;

        killed->UpdatePlayerSetting("mod-challenge-modes", HARDCORE_DEAD, 1);
    }

    void OnPlayerKilledByCreature(Creature* /*killer*/, Player* killed) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_HARDCORE, killed))
            return;

        killed->UpdatePlayerSetting("mod-challenge-modes", HARDCORE_DEAD, 1);
    }

    void OnPlayerResurrect(Player* player, float /*restore_percent*/, bool /*applySickness*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_HARDCORE, player))
            return;

        // Prevent resurrection attempts for dead hardcore characters
        // Ideally we'd deny the rez outright, but that needs a new core hook
        player->UpdatePlayerSetting("mod-challenge-modes", HARDCORE_DEAD, 1);
        player->KillPlayer();
        player->GetSession()->KickPlayer();
    }
};

// ============================================================================
// Semi-Hardcore mode
// ============================================================================

class ChallengeMode_SemiHardcore : public ChallengeMode
{
public:
    ChallengeMode_SemiHardcore() : ChallengeMode("ChallengeMode_SemiHardcore", SETTING_SEMI_HARDCORE) {}

    void OnPlayerKilledByCreature(Creature* /*killer*/, Player* player) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_SEMI_HARDCORE, player))
            return;

        ChatHandler handler(player->GetSession());
        for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        {
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                if (pItem->GetTemplate() && !pItem->IsEquipped())
                    continue;

                uint8 slot = pItem->GetSlot();
                handler.PSendSysMessage("|cffDA70D6You lost |cffffffff|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
                    pItem->GetEntry(), pItem->GetTemplate()->Name1.c_str());
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            }
        }
        player->SetMoney(0);
        handler.PSendSysMessage("|cffDA70D6You lost all your gear and gold!|r");
    }
};

// ============================================================================
// Self-Crafted mode
// ============================================================================

class ChallengeMode_SelfCrafted : public ChallengeMode
{
public:
    ChallengeMode_SelfCrafted() : ChallengeMode("ChallengeMode_SelfCrafted", SETTING_SELF_CRAFTED) {}

    bool OnPlayerCanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_SELF_CRAFTED, player))
            return true;

        if (!pItem->GetTemplate()->HasSignature())
            return false;

        return pItem->GetGuidValue(ITEM_FIELD_CREATOR) == player->GetGUID();
    }
};

// ============================================================================
// Low-Quality Gear mode
// ============================================================================

class ChallengeMode_ItemQualityLevel : public ChallengeMode
{
public:
    ChallengeMode_ItemQualityLevel() : ChallengeMode("ChallengeMode_ItemQualityLevel", SETTING_ITEM_QUALITY_LEVEL) {}

    bool OnPlayerCanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_ITEM_QUALITY_LEVEL, player))
            return true;

        return pItem->GetTemplate()->Quality <= ITEM_QUALITY_NORMAL;
    }
};

// ============================================================================
// Slow XP mode
// ============================================================================

class ChallengeMode_SlowXpGain : public ChallengeMode
{
public:
    ChallengeMode_SlowXpGain() : ChallengeMode("ChallengeMode_SlowXpGain", SETTING_SLOW_XP_GAIN) {}
};

// ============================================================================
// Very Slow XP mode
// ============================================================================

class ChallengeMode_VerySlowXpGain : public ChallengeMode
{
public:
    ChallengeMode_VerySlowXpGain() : ChallengeMode("ChallengeMode_VerySlowXpGain", SETTING_VERY_SLOW_XP_GAIN) {}
};

// ============================================================================
// Quest XP Only mode
// ============================================================================

class ChallengeMode_QuestXpOnly : public ChallengeMode
{
public:
    ChallengeMode_QuestXpOnly() : ChallengeMode("ChallengeMode_QuestXpOnly", SETTING_QUEST_XP_ONLY) {}

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_QUEST_XP_ONLY, player))
            return;

        if (victim)
        {
            // Still award XP to pets — they can't pass the player's level anyway
            if (Pet* pet = player->GetPet())
            {
                if (xpSource == XPSOURCE_KILL)
                    pet->GivePetXP(player->GetGroup() ? amount / 2 : amount);
            }
            amount = 0;
        }
        else
        {
            ChallengeMode::OnPlayerGiveXP(player, amount, victim, xpSource);
        }
    }
};

// ============================================================================
// Iron Man mode
// ============================================================================

class ChallengeMode_IronMan : public ChallengeMode
{
public:
    ChallengeMode_IronMan() : ChallengeMode("ChallengeMode_IronMan", SETTING_IRON_MAN) {}

    void OnPlayerResurrect(Player* player, float /*restore_percent*/, bool /*applySickness*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return;

        // Iron Man = no resurrection
        player->KillPlayer();
    }

    void OnPlayerLevelChanged(Player* player, uint8 oldlevel) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return;

        player->SetFreeTalentPoints(0);
        ChallengeMode::OnPlayerLevelChanged(player, oldlevel);
    }

    void OnPlayerTalentsReset(Player* player, bool /*noCost*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return;

        player->SetFreeTalentPoints(0);
    }

    bool OnPlayerCanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return true;

        return pItem->GetTemplate()->Quality <= ITEM_QUALITY_NORMAL;
    }

    bool OnPlayerCanApplyEnchantment(Player* player, Item* /*item*/, EnchantmentSlot /*slot*/, bool /*apply*/, bool /*apply_dur*/, bool /*ignore_condition*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return true;

        return false;
    }

    void OnPlayerLearnSpell(Player* player, uint32 spellID) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return;

        // Class skills are always allowed
        switch (spellID)
        {
            case RUNEFORGING:
            case POISONS:
            case BEAST_TRAINING:
                return;
            default:
                break;
        }

        // Block trade skills
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
        if (!spellInfo)
            return;

        for (uint8 i = 0; i < 3; i++)
        {
            if (spellInfo->Effects[i].Effect == SPELL_EFFECT_TRADE_SKILL)
            {
                player->removeSpell(spellID, SPEC_MASK_ALL, false);
                return;
            }
        }
    }

    bool OnPlayerCanUseItem(Player* player, ItemTemplate const* proto, InventoryResult& /*result*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return true;

        // Block potions, elixirs, flasks
        if (proto->Class == ITEM_CLASS_CONSUMABLE &&
            (proto->SubClass == ITEM_SUBCLASS_POTION ||
             proto->SubClass == ITEM_SUBCLASS_ELIXIR ||
             proto->SubClass == ITEM_SUBCLASS_FLASK))
        {
            return false;
        }

        // Block food that gives buffs
        if (proto->Class == ITEM_CLASS_CONSUMABLE && proto->SubClass == ITEM_SUBCLASS_FOOD)
        {
            for (uint8 i = 0; i < MAX_ITEM_SPELLS; i++)
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId);
                if (!spellInfo)
                    continue;

                for (uint8 j = 0; j < 3; j++)
                {
                    if (spellInfo->Effects[j].ApplyAuraName == SPELL_AURA_PERIODIC_TRIGGER_SPELL)
                        return false;
                }
            }
        }

        return true;
    }

    bool OnPlayerCanGroupInvite(Player* player, std::string& /*membername*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return true;

        return false;
    }

    bool OnPlayerCanGroupAccept(Player* player, Group* /*group*/) override
    {
        if (!sChallengeModes->challengeEnabledForPlayer(SETTING_IRON_MAN, player))
            return true;

        return false;
    }
};

// ============================================================================
// Player Login — sends active mode data for addon
// ============================================================================

class ChallengeModes_PlayerLogin : public PlayerScript
{
public:
    ChallengeModes_PlayerLogin() : PlayerScript("ChallengeModes_PlayerLogin") {}

    void OnPlayerLogin(Player* player) override
    {
        // Build a comma-separated list of active mode IDs and send as addon message
        std::string activeModes;
        auto const& allModes = ChallengeModes::GetAllModeInfos();
        for (auto const& mode : allModes)
        {
            if (player->GetPlayerSetting("mod-challenge-modes", mode.id).value)
            {
                if (!activeModes.empty())
                    activeModes.push_back(',');
                activeModes += std::to_string(mode.id);
            }
        }

        // Also send hardcore death flag if set
        if (player->GetPlayerSetting("mod-challenge-modes", HARDCORE_DEAD).value)
        {
            if (!activeModes.empty())
                activeModes.push_back(',');
            activeModes += std::to_string(HARDCORE_DEAD);
        }

        if (!activeModes.empty())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("CM_ACTIVE %s", activeModes.c_str());
        }
    }
};

// ============================================================================
// Shrine of Challenge game object
// ============================================================================

class gobject_challenge_modes : public GameObjectScript
{
public:
    gobject_challenge_modes() : GameObjectScript("gobject_challenge_modes") {}

    struct gobject_challenge_modesAI : GameObjectAI
    {
        explicit gobject_challenge_modesAI(GameObject* object) : GameObjectAI(object) {}

        bool CanBeSeen(Player const* player) override
        {
            if ((player->GetLevel() > 1 && player->getClass() != CLASS_DEATH_KNIGHT) || (player->GetLevel() > 55))
                return false;

            return sChallengeModes->enabled();
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_challenge_modesAI(object);
    }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        ChatHandler handler(player->GetSession());

        // Show currently active challenges
        auto const& allModes = ChallengeModes::GetAllModeInfos();
        bool hasActive = false;
        for (auto const& mode : allModes)
        {
            if (player->GetPlayerSetting("mod-challenge-modes", mode.id).value)
            {
                if (!hasActive)
                {
                    handler.PSendSysMessage("|cff00ff00=== Active Challenge Modes ===|r");
                    hasActive = true;
                }
                handler.PSendSysMessage("|cff00ff00  [ACTIVE] %s|r", mode.name.c_str());
            }
        }
        if (!hasActive)
        {
            handler.PSendSysMessage("|cffffcc00No challenge modes active yet. Select one below:|r");
        }

        // Show available options
        if (sChallengeModes->challengeEnabled(SETTING_HARDCORE) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_HARDCORE).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SEMI_HARDCORE).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Hardcore (one life only)", 0, SETTING_HARDCORE);
        }
        if (sChallengeModes->challengeEnabled(SETTING_SEMI_HARDCORE) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_HARDCORE).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SEMI_HARDCORE).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Semi-Hardcore (lose gear on death)", 0, SETTING_SEMI_HARDCORE);
        }
        if (sChallengeModes->challengeEnabled(SETTING_SELF_CRAFTED) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SELF_CRAFTED).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_IRON_MAN).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Self-Crafted (crafted gear only)", 0, SETTING_SELF_CRAFTED);
        }
        if (sChallengeModes->challengeEnabled(SETTING_ITEM_QUALITY_LEVEL) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_ITEM_QUALITY_LEVEL).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Low-Quality Gear (white/grey only)", 0, SETTING_ITEM_QUALITY_LEVEL);
        }
        if (sChallengeModes->challengeEnabled(SETTING_SLOW_XP_GAIN) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SLOW_XP_GAIN).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_VERY_SLOW_XP_GAIN).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Slow XP (0.5x rate)", 0, SETTING_SLOW_XP_GAIN);
        }
        if (sChallengeModes->challengeEnabled(SETTING_VERY_SLOW_XP_GAIN) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SLOW_XP_GAIN).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_VERY_SLOW_XP_GAIN).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Very Slow XP (0.25x rate)", 0, SETTING_VERY_SLOW_XP_GAIN);
        }
        if (sChallengeModes->challengeEnabled(SETTING_QUEST_XP_ONLY) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_QUEST_XP_ONLY).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Quest XP Only (no kill XP)", 0, SETTING_QUEST_XP_ONLY);
        }
        if (sChallengeModes->challengeEnabled(SETTING_IRON_MAN) &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_IRON_MAN).value &&
            !player->GetPlayerSetting("mod-challenge-modes", SETTING_SELF_CRAFTED).value)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Iron Man (no grouping/enchants/pots)", 0, SETTING_IRON_MAN);
        }

        SendGossipMenuFor(player, 12669, go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*sender*/, uint32 action) override
    {
        player->UpdatePlayerSetting("mod-challenge-modes", action, 1);

        ChatHandler handler(player->GetSession());
        handler.PSendSysMessage("|cff00ffChallenage mode '%s' has been activated! Good luck!|r",
            ChallengeModes::GetModeName(static_cast<ChallengeModeSettings>(action)).c_str());

        CloseGossipMenuFor(player);
        return true;
    }
};

// ============================================================================
// Script registration
// ============================================================================

void AddSC_mod_challenge_modes()
{
    new ChallengeModes_WorldScript();
    new ChallengeModes_PlayerLogin();
    new gobject_challenge_modes();
    new ChallengeMode_Hardcore();
    new ChallengeMode_SemiHardcore();
    new ChallengeMode_SelfCrafted();
    new ChallengeMode_ItemQualityLevel();
    new ChallengeMode_SlowXpGain();
    new ChallengeMode_VerySlowXpGain();
    new ChallengeMode_QuestXpOnly();
    new ChallengeMode_IronMan();
}
