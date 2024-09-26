// Copyright Cartesi and individual authors (see AUTHORS)
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along
// with this program (see COPYING). If not, see <https://www.gnu.org/licenses/>.
//

#include "clua-htif.h"
#include "clua-machine-util.h"
#include "clua.h"
#include "machine-c-api.h"
#include "riscv-constants.h"

namespace cartesi {

/// \brief This is the machine.get_default_machine_config()
/// method implementation.
static int machine_class_index_get_default_config(lua_State *L) {
    const char *config = cm_get_default_config();
    if (!config) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    clua_push_json(L, nlohmann::json::parse(config));
    return 1;
}

/// \brief This is the machine.verify_step_uarch_log() method implementation.
static int machine_class_index_verify_step_uarch_log(lua_State *L) {
    lua_settop(L, 2);
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 1)));
    if (cm_verify_step_uarch_log(managed_log.get(), true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief This is the machine.verify_step_uarch_state_transition() method implementation.
static int machine_class_index_verify_step_uarch_state_transition(lua_State *L) {
    lua_settop(L, 4);
    cm_hash root_hash{};
    clua_check_cm_hash(L, 1, &root_hash);
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 2)));
    cm_hash target_hash{};
    clua_check_cm_hash(L, 3, &target_hash);
    if (cm_verify_step_uarch_state_transition(&root_hash, managed_log.get(), &target_hash, true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief This is the machine.verify_reset_uarch_log() method implementation.
static int machine_class_index_verify_reset_uarch_log(lua_State *L) {
    lua_settop(L, 2);
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 1)));
    if (cm_verify_reset_uarch_log(managed_log.get(), true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief This is the machine.verify_reset_uarch_state_transition() method implementation.
static int machine_class_index_verify_reset_uarch_state_transition(lua_State *L) {
    lua_settop(L, 4);
    cm_hash root_hash{};
    clua_check_cm_hash(L, 1, &root_hash);
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 2)));
    cm_hash target_hash{};
    clua_check_cm_hash(L, 3, &target_hash);
    if (cm_verify_reset_uarch_state_transition(&root_hash, managed_log.get(), &target_hash, true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief This is the machine.get_csr_address() method implementation.
static int machine_class_index_get_csr_address(lua_State *L) {
    lua_pushinteger(L, static_cast<lua_Integer>(cm_get_csr_address(clua_check_cm_proc_csr(L, 1))));
    return 1;
}

/// \brief This is the machine.verify_send_cmio_response_log() method implementation.
static int machine_class_index_verify_send_cmio_response_log(lua_State *L) {
    lua_settop(L, 4);
    const uint16_t reason = static_cast<uint16_t>(luaL_checkinteger(L, 1));
    size_t length{0};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto *data = reinterpret_cast<const unsigned char *>(luaL_checklstring(L, 2, &length));
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 3)));
    if (cm_verify_send_cmio_response_log(reason, data, length, managed_log.get(), true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }
    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief This is the machine.verify_send_cmio_response_state_transition() method implementation.
static int machine_class_index_verify_send_cmio_response_state_transition(lua_State *L) {
    lua_settop(L, 6);
    cm_hash root_hash{};
    const uint16_t reason = static_cast<uint16_t>(luaL_checkinteger(L, 1));
    size_t length{0};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto *data = reinterpret_cast<const unsigned char *>(luaL_checklstring(L, 2, &length));
    clua_check_cm_hash(L, 3, &root_hash);
    auto &managed_log = clua_push_to(L, clua_managed_cm_ptr<cm_access_log>(clua_check_cm_access_log(L, 4)));
    cm_hash target_hash{};
    clua_check_cm_hash(L, 5, &target_hash);
    if (cm_verify_send_cmio_response_state_transition(reason, data, length, &root_hash, managed_log.get(), &target_hash,
            true) != 0) {
        return luaL_error(L, "%s", cm_get_last_error_message());
    }

    lua_pushnumber(L, 1);
    managed_log.reset();
    return 1;
}

/// \brief Contents of the machine class metatable __index table.
static const auto machine_class_index = cartesi::clua_make_luaL_Reg_array({
    {"get_default_config", machine_class_index_get_default_config},
    {"verify_step_uarch_log", machine_class_index_verify_step_uarch_log},
    {"verify_step_uarch_state_transition", machine_class_index_verify_step_uarch_state_transition},
    {"verify_reset_uarch_log", machine_class_index_verify_reset_uarch_log},
    {"verify_reset_uarch_state_transition", machine_class_index_verify_reset_uarch_state_transition},
    {"get_csr_address", machine_class_index_get_csr_address},
    {"verify_send_cmio_response_log", machine_class_index_verify_send_cmio_response_log},
    {"verify_send_cmio_response_state_transition", machine_class_index_verify_send_cmio_response_state_transition},
});

/// \brief This is the cartesi.machine() constructor implementation.
/// \param L Lua state.
static int machine_ctor(lua_State *L) {
    lua_settop(L, 3);
    auto &managed_machine = clua_push_to(L, clua_managed_cm_ptr<cm_machine>(nullptr));
    const std::string runtime_config = clua_value_to_json(L, 3).dump();
    if (lua_type(L, 2) == LUA_TTABLE) {
        const std::string config = clua_value_to_json(L, 2).dump();
        if (cm_create(config.c_str(), runtime_config.c_str(), &managed_machine.get()) != 0) {
            return luaL_error(L, "%s", cm_get_last_error_message());
        }
    } else {
        if (cm_load(luaL_checkstring(L, 2), runtime_config.c_str(), &managed_machine.get()) != 0) {
            return luaL_error(L, "%s", cm_get_last_error_message());
        }
    }
    return 1;
}

/// \brief Tag to identify the machine class-like constructor
struct machine_class {};

int clua_machine_init(lua_State *L, int ctxidx) {
    clua_createnewtype<clua_managed_cm_ptr<cm_access_log>>(L, ctxidx);
    clua_createnewtype<clua_managed_cm_ptr<cm_merkle_tree_proof>>(L, ctxidx);
    clua_createnewtype<clua_managed_cm_ptr<char>>(L, ctxidx);
    clua_createnewtype<clua_managed_cm_ptr<unsigned char>>(L, ctxidx);
    clua_createnewtype<clua_managed_cm_ptr<cm_memory_range_descr_array>>(L, ctxidx);
    if (!clua_typeexists<machine_class>(L, ctxidx)) {
        clua_createtype<machine_class>(L, "cartesi machine class", ctxidx);
        clua_setmethods<machine_class>(L, machine_class_index.data(), 0, ctxidx);
        static const auto machine_class_meta = cartesi::clua_make_luaL_Reg_array({
            {"__call", machine_ctor},
        });
        clua_setmetamethods<machine_class>(L, machine_class_meta.data(), 0, ctxidx);
        clua_gettypemetatable<machine_class>(L, ctxidx);
        lua_getfield(L, -1, "__index");
        clua_htif_export(L, ctxidx);
        lua_pop(L, 2);
    }
    return 1;
}

int clua_machine_export(lua_State *L, int ctxidx) {
    const int ctxabsidx = lua_absindex(L, ctxidx);
    // cartesi
    clua_machine_init(L, ctxabsidx);                    // cartesi
    lua_newtable(L);                                    // cartesi machine_class
    clua_setmetatable<machine_class>(L, -1, ctxabsidx); // cartesi machine_class
    lua_setfield(L, -2, "machine");                     // cartesi
    return 0;
}

} // namespace cartesi
