#pragma once

#include "Config.h"
#include "Identity.h"
#include "Sync.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <filesystem>
#include <set>
#include <string>
#include <thread>

class Launcher {
public:
    Launcher();
    ~Launcher();

    void set_port(int p);

    void check_for_updates(int argc, char** argv);

    void set_exe_name(const std::string& name) { m_exe_name = name; }
    void set_exe_path(const std::filesystem::path& path) { m_exe_path = path; }

    void find_game();

    void pre_game();

    void start_game();

    void start_network();

    std::string get_public_key();

private:
    /// Thread main function for the http(s) proxy thread.
    void proxy_main();

    /// Thread main function for the game thread.
    void game_main();

    /// Thread main function for the clien thread (the one that talks to the server)
    void tcp_client_main();

    void tcp_game_main();

    void udp_main();

    void auto_ping();

    void parse_config();

    static void check_mp(const std::string& path);

    void enable_mp();

    void net_core_main();

    void reset_status();

    void game_loop();

    void handle_core_packet(const std::vector<char>& RawData);

    bool start_sync(const std::string& Data);

    void server_parse(const std::string& data);

    void udp_send(const std::string& data);
    void server_send(const std::string& data, bool Res);
    void tcp_send(const std::string& data);
    void send_large(const std::string& data);
    void tcp_send(const std::vector<uint8_t>& data);
    std::string tcp_recv();
    std::string udp_recv();
    void game_send(const std::string& data);

    Sync<int> m_proxy_port;

    Sync<int> m_ping;

    Sync<int> m_client_id;

    Sync<bool> m_mod_loaded { false };

    Sync<Config> m_config;

    boost::scoped_thread<> m_proxy_thread { &Launcher::proxy_main, this };
    boost::scoped_thread<> m_game_thread;
    boost::scoped_thread<> m_client_thread;
    boost::scoped_thread<> m_tcp_game_thread;
    boost::scoped_thread<> m_udp_thread;
    boost::scoped_thread<> m_ping_thread;

    Sync<Identity> m_identity {};
    Sync<std::string> m_exe_name;
    Sync<std::filesystem::path> m_exe_path;
    boost::asio::io_context m_io {};
    boost::asio::ip::tcp::socket m_game_socket;
    boost::asio::ip::tcp::socket m_core_socket;
    boost::asio::ip::tcp::socket m_tcp_socket;
    boost::asio::ip::udp::socket m_udp_socket;
    boost::asio::ip::udp::endpoint m_udp_endpoint;
    Sync<bool> m_shutdown { false };
    Sync<std::chrono::high_resolution_clock::time_point> m_ping_start;
    Sync<std::chrono::high_resolution_clock::time_point> m_ping_end;

    Sync<std::string> m_m_status {};
    Sync<std::string> m_ul_status {};

    Sync<std::set<std::string>> m_conf_list;

    Sync<std::string> m_list_of_mods {};
};