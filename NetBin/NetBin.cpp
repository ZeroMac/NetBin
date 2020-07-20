// NetBin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define ORMPP_ENABLE_MYSQL
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/cinatra.hpp"



#include <boost/filesystem/path.hpp> 
#include <boost/filesystem/operations.hpp>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
using namespace cinatra;
using namespace std;
using namespace boost;
#include "../include/ormpp/dbng.hpp"
#include "../include/ormpp/mysql.hpp"
using namespace ormpp;
struct person
{
	int id;
	std::string name;
	int age;
};
REFLECTION(person, id, name, age);
//#include <mysql/mysql.h>


int main()
{
	person p = { 1, "test1", 2 };
	person p1 = { 2, "test2", 3 };
	person p2 = { 3, "test3", 4 };
	std::vector<person> v{ p1, p2 };

	dbng<mysql> mysql;
	mysql.connect("127.0.0.1", "root", "hlpassword", "hlsoft");
	//mysql.create_datatable<person>();

	mysql.insert(p);
	mysql.insert(v);

	mysql.update(p);
	mysql.update(v);

	auto result = mysql.query<person>(); //vector<person>
	for (auto& person : result) {
		std::cout << person.id << " " << person.name << " " << person.age << std::endl;
	}

	mysql.delete_records<person>();

	//transaction
	mysql.begin();
	for (int i = 0; i < 10; ++i) {
		person s = { i, "tom", 19 };
		if (!mysql.insert(s)) {
			mysql.rollback();
			return -1;
		}
	}
	mysql.commit();



	//exe 当前路径
	std::wstring exePath = boost::filesystem::initial_path<boost::filesystem::path>().wstring();
	std::wcout << exePath << endl;
	std::wcout << "Cinatra Server 启动!"<<endl;
	//uuid
	boost::uuids::random_generator gen;
	boost::uuids::uuid u;
	for (size_t i = 0; i < 10; i++)
	{
		u = gen();
		std::wcout << to_wstring(u) << endl;
	}
	// cinatra http 服务器
	http_server server(std::thread::hardware_concurrency());
	bool r = server.listen("0.0.0.0", "8090");
	if (!r) {
		//LOG_INFO << "listen failed";
		return -1;
	}
	server.set_http_handler<GET, POST>("/", [](request& req, response& res) mutable {
		res.set_status_and_content(status_type::ok, "hello world"); 
	});
	server.run();
    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
