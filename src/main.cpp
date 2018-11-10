#include <restinio/all.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using router_t = restinio::router::express_router_t<restinio::router::std_regex_engine_t>;

template <typename RESP>
RESP init_resp(RESP resp)
{
    resp.append_header(restinio::http_field::server, "medusa /v0.1");
    resp.append_header_date_field();
    return resp;
};

auto create_server_handler()
{
    auto router = std::make_unique<router_t>();

    router->http_get("/", [](auto req, auto params) {
        init_resp(req->create_response())
            .append_header(restinio::http_field::content_type, "text/plain; charset=utf-8")
            .set_body("Hello world!")
            .done();

        return restinio::request_accepted();
    });

    router->http_get("/json", [](auto req, auto params) {
        json j;
        j["message"] = "Hello world!";
        init_resp(req->create_response())
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(j.dump(4))
            .done();

        return restinio::request_accepted();
    });

    router->http_get("/html", [](auto req, auto params) {
        init_resp(req->create_response())
            .append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
            .set_body(
                "<html>\r\n"
                "    <head><title>Hello from RESTinio!</title></head>\r\n"
                "    <body>\r\n"
                "        <center><h1>Hello world</h1></center>\r\n"
                "    </body>\r\n"
                "</html>\r\n")
            .done();

        return restinio::request_accepted();
    });

    router->http_get("/file", [](auto req, auto params) {
        init_resp(req->create_response())
            .append_header(restinio::http_field::content_type, "text/plain; charset=utf-8")
            .set_body(restinio::sendfile("README.md"))
            .done();
        return restinio::request_accepted();
    });

    router->http_get("/single/:param", [](auto req, auto params) {
        return init_resp(req->create_response())
            .set_body(
                fmt::format(
                    "GET request with single parameter: '{}'",
                    params["param"]))
            .done();
    });

    router->non_matched_request_handler([](auto req) {
        return req->create_response(restinio::status_not_found())
            .append_header_date_field()
            .connection_close()
            .done();
        ;
    });

    return router;
}

int main(int argc, char const *argv[])
{
    using namespace std::chrono;

    try
    {
        using traits_t = restinio::traits_t<
            // TimerManager [asio_timer_manager_t, null_timer_manager_t]
            restinio::null_timer_manager_t,
            // Logger       [single_threaded_ostream_logger_t, shared_ostream_logger_t, null_logger_t]
            restinio::shared_ostream_logger_t,
            // Router       express_router_t<_regex_engine_t> [std, pcre, pcre2, boost]
            router_t>;
        restinio::run(
            restinio::on_thread_pool<traits_t>(8)
                .port(8080)
                .address("localhost")
                .request_handler(create_server_handler())
                .handle_request_timeout(1s)
                .cleanup_func([&] {
                    std::cout << "bye bye~" << std::endl;
                }));
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
