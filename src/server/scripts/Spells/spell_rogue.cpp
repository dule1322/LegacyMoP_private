/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum RogueSpells
{
    ROGUE_SPELL_SHIV_TRIGGERED                   = 5940,
    ROGUE_SPELL_DEADLY_POISON                    = 2823,
    ROGUE_SPELL_WOUND_POISON                     = 8679,
    ROGUE_SPELL_MIND_NUMBLING_POISON             = 5761,
    ROGUE_SPELL_CRIPPLING_POISON                 = 3408,
    ROGUE_SPELL_LEECHING_POISON                  = 108211,
    ROGUE_SPELL_PARALYTIC_POISON                 = 108215,
    ROGUE_SPELL_PARALYTIC_POISON_DEBUFF          = 113952,
    ROGUE_SPELL_DEBILITATING_POISON              = 115196,
    ROGUE_SPELL_MIND_PARALYSIS                   = 115194,
    ROGUE_SPELL_LEECH_VITALITY                   = 116921,
    ROGUE_SPELL_PARTIAL_PARALYSIS                = 115197,
    ROGUE_SPELL_TOTAL_PARALYSIS                  = 113953,
    ROGUE_SPELL_DEADLY_POISON_DOT                = 2818,
    ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE     = 113780,
    ROGUE_SPELL_SLICE_AND_DICE                   = 5171,
    ROGUE_SPELL_SMOKE_BOMB_AREA_DUMMY            = 76577,
    ROGUE_SPELL_SMOKE_BOMB_AURA                  = 88611,
    ROGUE_SPELL_MASTER_POISONER_AURA             = 58410,
    ROGUE_SPELL_MASTER_POISONER_DEBUFF           = 93068,
    ROGUE_SPELL_CRIMSON_TEMPEST_DOT              = 122233,
    ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA       = 115834,
    ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE            = 51637,
    ROGUE_SPELL_VENOMOUS_WOUND_DAMAGE            = 79136,
    ROGUE_SPELL_GARROTE_DOT                      = 703,
    ROGUE_SPELL_RUPTURE_DOT                      = 1943,
    ROGUE_SPELL_CUT_TO_THE_CHASE_AURA            = 51667,
    ROGUE_SPELL_HEMORRHAGE_DOT                   = 89775,
    ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF           = 124271,
    ROGUE_SPELL_NIGHTSTALKER_AURA                = 14062,
    ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE         = 130493,
    ROGUE_SPELL_NERVE_STRIKE_AURA                = 108210,
    ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE  = 112947,
    ROGUE_SPELL_COMBAT_READINESS                 = 74001,
    ROGUE_SPELL_COMBAT_INSIGHT                   = 74002,
    ROGUE_SPELL_BLADE_FLURRY_DAMAGE              = 22482,
    ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE        = 45182,
};

// Shuriken Toss - 114014
class spell_rog_shuriken_toss : public SpellScriptLoader
{
    public:
        spell_rog_shuriken_toss() : SpellScriptLoader("spell_rog_shuriken_toss") { }

        class spell_rog_shuriken_toss_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shuriken_toss_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                    if (Unit* caster = GetCaster())
                        if (target->IsInRange(caster, 10.0f, 100.0f) && !caster->HasAura(121471))
                            caster->CastSpell(caster, 137586, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shuriken_toss_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shuriken_toss_SpellScript();
        }
};

// Cheat Death - 31230
class spell_rog_cheat_death : public SpellScriptLoader
{
    public:
        spell_rog_cheat_death() : SpellScriptLoader("spell_rog_cheat_death") { }

        class spell_rog_cheat_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_cheat_death_AuraScript);

            void CalculateAmount(AuraEffect const* /*AuraEffect**/, float& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (Unit* target = GetTarget())
                {
                    if (dmgInfo.GetDamage() < target->GetHealth())
                        return;

                    if (target->ToPlayer()->HasAura(45181))
                        return;

                    target->CastSpell(target, ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE, true);
                    target->CastSpell(target, 45181, true);

                    uint32 health10 = target->CountPctFromMaxHealth(10);

                    // hp > 10% - absorb hp till 10%
                    if (target->GetHealth() > health10)
                        absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health10;
                    // hp lower than 10% - absorb everything
                    else
                        absorbAmount = dmgInfo.GetDamage();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_cheat_death_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_cheat_death_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_cheat_death_AuraScript();
        }
};

// Blade Flurry - 13877
class spell_rog_blade_flurry : public SpellScriptLoader
{
    public:
        spell_rog_blade_flurry() : SpellScriptLoader("spell_rog_blade_flurry") { }

        class spell_rog_blade_flurry_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_blade_flurry_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    float damage = eventInfo.GetDamageInfo()->GetDamage();
                    SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();

                    if (!damage || eventInfo.GetDamageInfo()->GetDamageType() == DOT)
                        return;

                    int32 percent = GetSpellInfo()->Effects[2]->BasePoints;
                    damage = int32((damage * percent) / 100);

                    if (Unit* target = _player->SelectNearbyTarget(eventInfo.GetActionTarget()))
                        _player->CastCustomSpell(target, ROGUE_SPELL_BLADE_FLURRY_DAMAGE, &damage, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_blade_flurry_AuraScript();
        }
};

// Growl - 113613
class spell_rog_growl : public SpellScriptLoader
{
    public:
        spell_rog_growl() : SpellScriptLoader("spell_rog_growl") { }

        class spell_rog_growl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_growl_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;

                if (!GetHitUnit())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = _player->GetSelectedUnit())
                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, 355, true); // Taunt
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_growl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_growl_SpellScript();
        }
};

// Cloak of Shadows - 31224 and Cloak of Shadows - 110788 (Symbiosis)
class spell_rog_cloak_of_shadows : public SpellScriptLoader
{
    public:
        spell_rog_cloak_of_shadows() : SpellScriptLoader("spell_rog_cloak_of_shadows") { }

        class spell_rog_cloak_of_shadows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cloak_of_shadows_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;
                const SpellInfo* m_spellInfo = GetSpellInfo();

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    uint32 dispelMask = SpellInfo::GetDispelMask(DISPEL_ALL);
                    Unit::AuraApplicationMap& Auras = _player->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end();)
                    {
                        // remove all harmful spells on you...
                        SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
                        if ((spell->DmgClass == SPELL_DAMAGE_CLASS_MAGIC // only affect magic spells
                            || (spell->GetDispelMask() & dispelMask) || (spell->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                            // ignore positive and passive auras
                            && !iter->second->IsPositive() && !iter->second->GetBase()->IsPassive() && m_spellInfo->CanDispelAura(spell))
                        {
                            _player->RemoveAura(iter);
                        }
                        else
                            ++iter;
                    }
                    return;
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cloak_of_shadows_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cloak_of_shadows_SpellScript();
        }
};

// Combat Readiness - 74001
class spell_rog_combat_readiness : public SpellScriptLoader
{
    public:
        spell_rog_combat_readiness() : SpellScriptLoader("spell_rog_combat_readiness") { }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            uint32 update;
            bool hit;

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (GetCaster())
                {
                    update = 10000;
                    hit = false;
                }
            }

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                update -= diff;

                if (GetCaster())
                    if (GetCaster()->HasAura(ROGUE_SPELL_COMBAT_INSIGHT))
                        hit = true;

                if (update <= 0)
                    if (Player* _player = GetCaster()->ToPlayer())
                        if (!hit)
                            _player->RemoveAura(ROGUE_SPELL_COMBAT_READINESS);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_combat_readiness_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_rog_combat_readiness_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

// Called by Kidney Shot - 408 and Cheap Shot - 1833
// Nerve Strike - 108210
class spell_rog_nerve_strike : public SpellScriptLoader
{
    public:
        spell_rog_nerve_strike() : SpellScriptLoader("spell_rog_nerve_strike") { }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (GetCaster() && GetTarget())
                    if (GetCaster()->HasAura(ROGUE_SPELL_NERVE_STRIKE_AURA))
                        GetCaster()->CastSpell(GetTarget(), ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE, true);
            }

            void CalculateMaxDuration(int32& duration)
            {
                if (GetId() == 1833) // don't increase duration for Cheap Shot
                    return;

                Unit* caster = GetCaster();
                Unit* target = GetUnitOwner();
                if (!caster || !target)
                    return;

                if (AuraEffect* aurEff = target->GetAuraEffect(84617, 2, caster->GetGUID()))
                    AddPct(duration, aurEff->GetAmount());
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_combat_readiness_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_rog_combat_readiness_AuraScript::CalculateMaxDuration);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

// Called by Stealth - 1784
// Nightstalker - 14062
class spell_rog_nightstalker : public SpellScriptLoader
{
    public:
        spell_rog_nightstalker() : SpellScriptLoader("spell_rog_nightstalker") { }

        class spell_rog_nightstalker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_nightstalker_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(ROGUE_SPELL_NIGHTSTALKER_AURA))
                        caster->CastSpell(caster, ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_nightstalker_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_nightstalker_SpellScript();
        }

        class spell_rog_nightstalker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_nightstalker_AuraScript);

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* aura = caster->GetAura(ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE))
                        aura->Remove();
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_rog_nightstalker_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_nightstalker_AuraScript();
        }
};

// Hemorrhage - 16511
class spell_rog_hemorrhage : public SpellScriptLoader
{
    public:
        spell_rog_hemorrhage() : SpellScriptLoader("spell_rog_hemorrhage") { }

        class spell_rog_hemorrhage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_hemorrhage_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                    if (Player* _player = caster->ToPlayer())
                        if (Unit* target = GetHitUnit())
                        {
                            if(_player->HasAura(56807) && !target->HasAuraWithMechanic((1 << MECHANIC_BLEED))) //Glyph of Hemorrhage
                                return;

                            float bp = CalculatePct(float(GetHitDamage()) + GetHitAbsorb(), GetSpellInfo()->Effects[EFFECT_3]->BasePoints) / 8;
                            _player->CastCustomSpell(target, ROGUE_SPELL_HEMORRHAGE_DOT, &bp, NULL, NULL, true);
                        }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_rog_hemorrhage_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_hemorrhage_SpellScript();
        }
};

// Rupture - 1943
// Venomous Wounds - 79134
class spell_rog_venomous_wounds : public SpellScriptLoader
{
    public:
        spell_rog_venomous_wounds() : SpellScriptLoader("spell_rog_venomous_wounds") { }

        class spell_rog_venomous_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_venomous_wounds_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                        if (removeMode == AURA_REMOVE_BY_DEATH)
                        {
                            if (Aura* rupture = aurEff->GetBase())
                            {
                                // If an enemy dies while afflicted by your Rupture, you regain energy proportional to the remaining Rupture duration
                                float duration = float(rupture->GetDuration() / 1000.0f);
                                caster->CastCustomSpell(caster, ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE, &duration, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_venomous_wounds_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_venomous_wounds_AuraScript();
        }
};

// Redirect - 73981 and Redirect - 110730
class spell_rog_redirect : public SpellScriptLoader
{
    public:
        spell_rog_redirect() : SpellScriptLoader("spell_rog_redirect") { }

        class spell_rog_redirect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_redirect_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* caster = GetCaster())
                {
                    Player* plr = caster->ToPlayer();
                    Unit* target = GetExplTargetUnit();

                    if (!plr || !target)
                        return SPELL_FAILED_DONT_REPORT;
                    
                    if (!plr->GetComboPoints())
                        return SPELL_FAILED_NO_COMBO_POINTS;

                    if (target->GetGUID() == plr->GetComboTarget())
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        uint8 cp = _player->GetComboPoints();

                        if (cp > 5)
                            cp = 5;

                        _player->ClearComboPoints(true);
                        _player->AddComboPoints(target, cp, GetSpell());
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_redirect_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_rog_redirect_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_redirect_SpellScript();
        }
};

// Shroud of Concealment - 115834
class spell_rog_shroud_of_concealment : public SpellScriptLoader
{
    public:
        spell_rog_shroud_of_concealment() : SpellScriptLoader("spell_rog_shroud_of_concealment") { }

        class spell_rog_shroud_of_concealment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shroud_of_concealment_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Aura* shroudOfConcealment = target->GetAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, _player->GetGUID()))
                            if (!target->IsInRaidWith(_player) && !target->IsInPartyWith(_player))
                                target->RemoveAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, _player->GetGUID());
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shroud_of_concealment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shroud_of_concealment_SpellScript();
        }
};

// Crimson Tempest - 121411
class spell_rog_crimson_tempest : public SpellScriptLoader
{
    public:
        spell_rog_crimson_tempest() : SpellScriptLoader("spell_rog_crimson_tempest") { }

        class spell_rog_crimson_tempest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_crimson_tempest_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                    if (Player* _player = caster->ToPlayer())
                        if (Unit* target = GetHitUnit())
                        {
                            int32 percent = 240;
                            if(SpellInfo const* sinfo = sSpellMgr->GetSpellInfo(ROGUE_SPELL_CRIMSON_TEMPEST_DOT))
                                percent = sinfo->Effects[0]->BasePoints;

                            float damage = float((GetHitDamage() + GetHitAbsorb()) / 6);
                            AddPct(damage, percent);
                            _player->CastCustomSpell(target, ROGUE_SPELL_CRIMSON_TEMPEST_DOT, &damage, NULL, NULL, true);
                        }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_rog_crimson_tempest_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_crimson_tempest_SpellScript();
        }
};

// Called by Wound Poison - 8680, Deadly Poison - 2818, Mind-Numbing Poison - 5760, Crippling Poison - 3409
// Paralytic Poison - 113952, Leeching Poison - 112961 and Deadly Poison : Instant damage - 113780
// Master Poisoner - 58410
class spell_rog_master_poisoner : public SpellScriptLoader
{
    public:
        spell_rog_master_poisoner() : SpellScriptLoader("spell_rog_master_poisoner") { }

        class spell_rog_master_poisoner_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_master_poisoner_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(ROGUE_SPELL_MASTER_POISONER_AURA))
                            _player->CastSpell(target, ROGUE_SPELL_MASTER_POISONER_DEBUFF, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_master_poisoner_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_master_poisoner_SpellScript();
        }
};

// Called by Deadly Poison - 2818
// Deadly Poison : Instant damage - 113780
class spell_rog_deadly_poison_instant_damage : public SpellScriptLoader
{
    public:
        spell_rog_deadly_poison_instant_damage() : SpellScriptLoader("spell_rog_deadly_poison_instant_damage") { }

        class spell_rog_deadly_poison_instant_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_poison_instant_damage_SpellScript);

            void HandleOnCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        if (target->HasAura(ROGUE_SPELL_DEADLY_POISON_DOT, _player->GetGUID()))
                            _player->CastSpell(target, ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE, true);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_rog_deadly_poison_instant_damage_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_poison_instant_damage_SpellScript();
        }
};

// Paralytic Poison - 113952
class spell_rog_paralytic_poison : public SpellScriptLoader
{
    public:
        spell_rog_paralytic_poison() : SpellScriptLoader("spell_rog_paralytic_poison") { }

        class spell_rog_paralytic_poison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_paralytic_poison_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* paralyticPoison = target->GetAura(ROGUE_SPELL_PARALYTIC_POISON_DEBUFF))
                        {
                            if (paralyticPoison->GetStackAmount() == 4 && !target->HasAura(ROGUE_SPELL_TOTAL_PARALYSIS))
                            {
                                _player->CastSpell(target, ROGUE_SPELL_TOTAL_PARALYSIS, true);
                                target->RemoveAura(ROGUE_SPELL_PARALYTIC_POISON_DEBUFF);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_paralytic_poison_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_paralytic_poison_SpellScript();
        }
};

// Shiv - 5938
class spell_rog_shiv : public SpellScriptLoader
{
    public:
        spell_rog_shiv() : SpellScriptLoader("spell_rog_shiv") { }

        class spell_rog_shiv_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shiv_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(ROGUE_SPELL_MIND_NUMBLING_POISON))
                            _player->CastSpell(target, ROGUE_SPELL_MIND_PARALYSIS, true);
                        else if (_player->HasAura(ROGUE_SPELL_CRIPPLING_POISON))
                            _player->CastSpell(target, ROGUE_SPELL_DEBILITATING_POISON, true);
                        else if (_player->HasAura(ROGUE_SPELL_LEECHING_POISON))
                            _player->CastSpell(_player, ROGUE_SPELL_LEECH_VITALITY, true);
                        else if (_player->HasAura(ROGUE_SPELL_PARALYTIC_POISON))
                            _player->CastSpell(target, ROGUE_SPELL_PARTIAL_PARALYSIS, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shiv_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shiv_SpellScript();
        }
};

// Shadow Step - 36554
class spell_rog_shadowstep : public SpellScriptLoader
{
    public:
        spell_rog_shadowstep() : SpellScriptLoader("spell_rog_shadowstep") { }

        class spell_rog_shadowstep_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shadowstep_SpellScript);

            SpellCastResult CheckCast()
            {
                if (GetCaster()->HasUnitState(UNIT_STATE_ROOT))
                    return SPELL_FAILED_ROOTED;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_shadowstep_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shadowstep_SpellScript();
        }
};

class spell_rog_eviscerate : public SpellScriptLoader
{
    public:
        spell_rog_eviscerate() : SpellScriptLoader("spell_rog_eviscerate") { }

        class spell_rog_eviscerate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_eviscerate_SpellScript)

            bool Validate(SpellInfo const * /*SpellInfo*/)
            {
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if (Unit * caster = GetCaster())
                {
                    if (caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        caster->ToPlayer()->KilledMonsterCredit(44175, 0);
                        caster->ToPlayer()->KilledMonsterCredit(44548, 0);
                    }
                    if (target && caster->HasAura(14172))
                    {
                        uint8 comboPoint = caster->m_movedPlayer ? caster->m_movedPlayer->GetComboPoints() : 1;
                        uint32 chance = 10 * comboPoint;
                        if(caster->HasAura(14172))
                            chance *= 2;
                        if(roll_chance_i(chance))
                            if(Aura* aura = target->GetAura(1943, caster->GetGUID()))
                                aura->RefreshDuration();
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_eviscerate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_eviscerate_SpellScript();
        }
};

// Distract  - 1725
class spell_rog_distract : public SpellScriptLoader
{
    public:
        spell_rog_distract() : SpellScriptLoader("spell_rog_distract") { }

        class spell_rog_distract_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_distract_SpellScript);

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(146961)) // Glyph of Improved Distraction
                    {
                        Position pos;
                        GetExplTargetDest()->GetPosition(&pos);
                        TempSummon* summon = caster->SummonCreature(73544, pos, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 5000);
                        if (!summon)
                            return;

                        summon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, caster->GetGUID());
                        summon->AddAura(31366, summon);
                        //summon->SetMaxHealth(caster->CountPctFromMaxHealth(50));
                        //summon->SetHealth(summon->GetMaxHealth());
                    }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_rog_distract_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_distract_SpellScript();
        }
};

// Kick - 1766
class spell_rog_kick : public SpellScriptLoader
{
    public:
        spell_rog_kick() : SpellScriptLoader("spell_rog_kick") { }

        class spell_rog_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_kick_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if(_player->HasAura(56805) && GetSpell()->GetInterupted())
                        _player->ModifySpellCooldown(GetSpellInfo()->Id, -6000);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_rog_kick_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_kick_SpellScript();
        }
};

// Pick Pocket - 921
class spell_rog_pick_pocket : public SpellScriptLoader
{
    public:
        spell_rog_pick_pocket() : SpellScriptLoader("spell_rog_pick_pocket") { }

        class spell_rog_pick_pocket_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_pick_pocket_SpellScript);

            void HandleOnCast()
            {
                if(Unit* caster = GetCaster())
                    if(Unit* target = GetExplTargetUnit())
                        if(!caster->HasAura(63268))
                            target->CastSpell(caster, 121308, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_rog_pick_pocket_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_pick_pocket_SpellScript();
        }
};

// Killing Spree - 51690
class spell_rog_killing_spree : public SpellScriptLoader
{
    public:
        spell_rog_killing_spree() : SpellScriptLoader("spell_rog_killing_spree") { }

        class spell_rog_killing_spree_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_killing_spree_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = _player->GetSelectedUnit())
                        if (_player->IsValidAttackTarget(target))
                            if (_player->IsWithinDist(target, 10.0f))
                                return SPELL_CAST_OK;

                return SPELL_FAILED_BAD_TARGETS;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                std::list<WorldObject*> targetTemp = targets;
                targets.clear();

                if (Unit* caster = GetCaster())
                {
                    if(caster->HasAura(13877))
                    {
                        if (targetTemp.size() > 6)
                            targetTemp.resize(6);
                    }
                    else
                    {
                        if (Player* _player = caster->ToPlayer())
                        {
                            if (Unit* target = _player->GetSelectedUnit())
                            {
                                targetTemp.clear();
                                targetTemp.push_back(target);
                            }
                            else
                                targetTemp.resize(1);
                        }
                    }
                }

                std::list<uint64> targetList;
                for (std::list<WorldObject*>::iterator itr = targetTemp.begin(); itr != targetTemp.end(); ++itr)
                    if(WorldObject* object = (*itr))
                        targetList.push_back(object->GetGUID());

                GetSpell()->SetEffectTargets(targetList);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_killing_spree_SpellScript::CheckCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_killing_spree_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_killing_spree_SpellScript();
        }

        class spell_rog_killing_spree_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_killing_spree_AuraScript);

            Position pos;

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                //Rogue T16 4P Bonus for Killing Spree
                if (GetCaster()->HasAura(145210))
                    if (aurEff->GetTickNumber() >= 2)
                        if (AuraEffect* aurEffb = GetCaster()->GetAura(145210)->GetEffect(0))
                            aurEffb->SetAmount(aurEffb->GetAmount() + 10); //increase damage for next killing spree on 10%

                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetTarget();
                    std::list<uint64> targets = GetAura()->GetEffectTargets();

                    if (!targets.empty())
                    {
                        uint64 targetGuid = Trinity::Containers::SelectRandomContainerElement(targets);
                        if (Unit* effectTarget = ObjectAccessor::GetUnit(*caster, targetGuid))
                            target = effectTarget;
                    }
                    if (target)
                    {
                        caster->CastSpell(target, 57840, true);
                        caster->CastSpell(target, 57841, true);
                    }
                }
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (GetCaster()->HasAura(145210))
                    if (AuraEffect* aurEffb = GetCaster()->GetAura(145210)->GetEffect(0))
                        aurEffb->SetAmount(10); ////Reset value, for safe

                GetCaster()->CastSpell(GetCaster(), 61851, true);
                GetCaster()->GetPosition(&pos);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(145210))
                        if (AuraEffect* aurEffb = caster->GetAura(145210)->GetEffect(0))
                            aurEffb->SetAmount(10); //Reset value

                    if (caster->HasAura(63252))
                        caster->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_killing_spree_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectApply += AuraEffectApplyFn(spell_rog_killing_spree_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_killing_spree_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_killing_spree_AuraScript();
        }
};

// Blade Flurry AOE - 22482
class spell_rog_blade_flurry_aoe : public SpellScriptLoader
{
    public:
        spell_rog_blade_flurry_aoe() : SpellScriptLoader("spell_rog_blade_flurry_aoe") { }

        class spell_rog_blade_flurry_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_blade_flurry_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if(Unit* target = _player->GetSelectedUnit())
                        targets.remove(target);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_blade_flurry_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_blade_flurry_aoe_SpellScript();
        }
};

// Fan of Knives - 51723
class spell_rog_fan_of_knives_cp : public SpellScriptLoader
{
public:
    spell_rog_fan_of_knives_cp() : SpellScriptLoader("spell_rog_fan_of_knives_cp") { }

    class spell_rog_fan_of_knives_cp_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_fan_of_knives_cp_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            if (Unit* caster = GetCaster())
                if (Player* plr = caster->ToPlayer())
                {
                    Unit* target = NULL;

                    if (uint64 comboTargetGUID = plr->GetComboTarget())
                        for (auto itr : targets)
                            if (itr && itr->GetGUID() == comboTargetGUID)
                                if (Unit * comdoTarget = ObjectAccessor::GetUnit(*plr, plr->GetComboTarget()))
                                    target = comdoTarget;

                    if (!target)
                        if (Unit* selectedTarget = plr->GetSelectedUnit())
                            for (auto itr : targets)
                                if (itr && itr->GetGUID() == selectedTarget->GetGUID())
                                    target = selectedTarget;

                    if (target)
                    {
                        targets.clear();
                        targets.push_back(target);
                    }
                    else
                        targets.resize(1);
                }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_fan_of_knives_cp_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_fan_of_knives_cp_SpellScript();
    }
};

void AddSC_rogue_spell_scripts()
{
    new spell_rog_shuriken_toss();
    new spell_rog_cheat_death();
    new spell_rog_blade_flurry();
    new spell_rog_growl();
    new spell_rog_cloak_of_shadows();
    new spell_rog_combat_readiness();
    new spell_rog_nerve_strike();
    new spell_rog_nightstalker();
    new spell_rog_hemorrhage();
    new spell_rog_venomous_wounds();
    new spell_rog_redirect();
    new spell_rog_shroud_of_concealment();
    new spell_rog_crimson_tempest();
    new spell_rog_master_poisoner();
    new spell_rog_deadly_poison_instant_damage();
    new spell_rog_paralytic_poison();
    new spell_rog_shiv();
    new spell_rog_shadowstep();
    new spell_rog_eviscerate();
    new spell_rog_distract();
    new spell_rog_kick();
    new spell_rog_pick_pocket();
    new spell_rog_killing_spree();
    new spell_rog_blade_flurry_aoe();
    new spell_rog_fan_of_knives_cp();
}
