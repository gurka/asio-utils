/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Simon Sandström
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ASIOUTILS_ACCEPTOR_H_
#define ASIOUTILS_ACCEPTOR_H_

#include <functional>
#include <memory>
#include <utility>

#include <asio.hpp>

namespace asioutils
{

class Acceptor
{
 public:
  Acceptor(asio::io_context* io_context,
           int port,
           std::function<void(asio::ip::tcp::socket&&)> handler)
    : m_acceptor(*io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      m_socket(*io_context),
      m_handler(std::move(handler))
  {
  }

  Acceptor(const Acceptor&) = delete;
  Acceptor& operator=(const Acceptor&) = delete;

  void start()
  {
    accept();
  }

  void stop()
  {
    m_acceptor.close();
  }

 private:
  void accept()
  {
    m_acceptor.async_accept(m_socket, [this](const std::error_code& error_code)
    {
      if (!m_acceptor.is_open())
      {
        return;
      }

      if (!error_code)
      {
        m_handler(std::move(m_socket));
      }

      accept();
    });
  }

  asio::ip::tcp::acceptor m_acceptor;
  asio::ip::tcp::socket m_socket;
  std::function<void(asio::ip::tcp::socket&&)> m_handler;
};

}  // namespace asioutils

#endif  // ASIOUTILS_ACCEPTOR_H_
