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

#ifndef JSONRPC_CONNECTION_H
#define JSONRPC_CONNECTION_H

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "asio-config.h" // must be included before any ASIO header
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/container/static_vector.hpp>
#pragma GCC diagnostic pop

namespace cartesi {

class jsonrpc_connection final {
public:
    enum class manage { server, machine, none };
    explicit jsonrpc_connection(std::string remote_address, manage what);
    jsonrpc_connection(const jsonrpc_connection &other) = delete;
    jsonrpc_connection(jsonrpc_connection &&other) noexcept = delete;
    jsonrpc_connection &operator=(const jsonrpc_connection &other) = delete;
    jsonrpc_connection &operator=(jsonrpc_connection &&other) noexcept = delete;
    ~jsonrpc_connection();
    bool is_forked(void) const;
    bool is_shutdown(void) const;
    boost::beast::tcp_stream &get_stream(void);
    const boost::beast::tcp_stream &get_stream(void) const;
    const std::string &get_remote_address(void) const;
    const std::string &get_remote_parent_address(void) const;
    void snapshot(void);
    void commit(void);
    void rollback(void);
    manage get_what_managed(void) const;

private:
    void shutdown(void);
    boost::asio::io_context m_ioc{1};         // The io_context is required for all I/O
    boost::beast::tcp_stream m_stream{m_ioc}; // TCP stream for keep alive connections
    boost::container::static_vector<std::string, 2> m_address{};
    manage m_what_managed{manage::server};
};

} // namespace cartesi

#endif