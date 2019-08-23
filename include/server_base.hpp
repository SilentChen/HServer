#ifndef SERVER_BASE_HPP
#define SERVER_BASE_HPP

#include <boost/asio.hpp>

#include <regex>
#include <unordered_map>
#include <thread>

namespace HServer {
	struct Request {
		std::string method, path, http_version;
		// content use the counting ptr to record the used times.
		std::shared_ptr<std::istream> content;
		std::unordered_map<std::string, std::string> header;
		std::smatch path_match;
	};
	typedef std::map<std::string, std::unordered_map<std::string, std::function<void(std::ostream&, Request&)>>> resource_type;
	template <typename socket_type>
	class ServerBase {
		public:
			resource_type resource;
			resource_type default_resource;

			ServerBase(unsigned short port, size_t num_threads = 1) : endpoint(boost::asio::ip::tcp::v4(), port), acceptor(m_io_service, endpoint), num_threads(num_threads) {}

			void debug();

			//
			void start() {
				for (auto it = resource.begin(); it != resource.end(); it++) {
					all_resources.push_back(it);	
				}
				for (auto it=default_resource.begin(); it != default_resource.end(); it ++) {
					all_resources.push_back(it);
				}

				accept();

				for(size_t c=1;c<num_threads;c++) {
					threads.emplace_back([this](){
							m_io_service.run();
					});
				}


				m_io_service.run();

				for (auto& t : threads)
					t.join();
			}
		protected:
			// asio 库中的 io_service 是调度器，所有的异步 IO 事件都要通过它来分发处理
			// 换句话说, 需要 IO 的对象的构造函数，都需要传入一个 io_service 对象
			boost::asio::io_service m_io_service; 
			// IP 地址、端口号、协议版本构成一个 endpoint，并通过这个 endpoint 在服务端生成
			// tcp::acceptor 对象，并在指定端口上等待连接
			boost::asio::ip::tcp::endpoint endpoint;
			// 所以，一个 acceptor 对象的构造都需要 io_service 和 endpoint 两个参数
			boost::asio::ip::tcp::acceptor acceptor;
			
			size_t num_threads;

			std::vector<std::thread> threads;

			 // 所有的资源及默认资源都会在 vector 尾部添加, 并在 start() 中创建
			std::vector<resource_type::iterator> all_resources;

			// 需要不同类型服务器实现这个方法
			virtual void accept() {}

			//
			Request parse_request(std::istream& stream) const {
				Request request;
				std::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
				std::smatch sub_match;
				std::string line;
				getline(stream, line);
				line.pop_back();
				if (std::regex_match(line, sub_match, e)) {
					request.method		= sub_match[1];
					request.path		= sub_match[2];
					request.http_version= sub_match[3];
					bool matched;
					e="^([^:]*): ?(.*)$";
					do {
						getline(stream, line);
						line.pop_back();
						matched=std::regex_match(line, sub_match, e);
						if (matched) {
							request.header[sub_match[1]] = sub_match[2];
						}
					} while (matched == true);
				}
				return request;
			}

			//
			void process_request_and_respond(std::shared_ptr<socket_type> socket) const {
				auto read_buffer = std::make_shared<boost::asio::streambuf>();

				boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n", [this, socket, read_buffer](const boost::system::error_code& ec,size_t bytes_transferred) {
						if (!ec) {
							size_t total = read_buffer->size();
							std::istream stream(read_buffer.get());
							auto request = std::make_shared<Request>();
							*request = parse_request(stream);
							size_t num_additional_bytes = total-bytes_transferred;

							if (request -> header.count("Content-Length")>0) {
								boost::asio::async_read(*socket, *read_buffer, boost::asio::transfer_exactly(stoull(request->header["Content-Length"]) - num_additional_bytes), [this, socket, read_buffer, request](const boost::system::error_code& ec, size_t bytes_transferred){
									if (!ec) {
										request -> content = std::shared_ptr<std::istream>(new std::istream(read_buffer.get()));
										respond(socket, request);
									}	
								});
							} else {
								respond(socket, request);
							}
						}		
				});
			}

			//
			void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
				for (auto res_it : all_resources) {
					std::regex e(res_it -> first);
					std::smatch sm_res;
					if (std::regex_match(request -> path, sm_res, e)) {
						if (res_it -> second.count(request->method) > 0 ) {
							request -> path_match = move(sm_res);

							auto write_buffer = std::make_shared<boost::asio::streambuf>();
							std::ostream response(write_buffer.get());
							res_it -> second[request -> method](response, *request);

							boost::asio::async_write(*socket, *write_buffer, [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred){
								if (!ec && stof(request->http_version) > 1.05)
									process_request_and_respond(socket);
							});
							return;
						}
					}
				}
			}
	};

	template<typename socket_type>
	void ServerBase<socket_type>::debug() {
	
	}

	template <typename socket_type>
	class Server : public ServerBase<socket_type> {};
}

#endif /* SERVER_BASE_HPP  */
