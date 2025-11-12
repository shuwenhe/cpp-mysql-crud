#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

int main() {
    try {
        // 1️⃣ 获取 MySQL 驱动
        sql::Driver* driver = get_driver_instance();

        // 2️⃣ 建立连接
        unique_ptr<Connection> con(driver->connect("tcp://127.0.0.1:3306", "shuwen", "shuwen"));
        cout << "✅ Connected to MySQL successfully!" << endl;

        // 3️⃣ 选择数据库
        con->setSchema("shuwen");  // 请确保 test 数据库已创建：CREATE DATABASE test;

        // 4️⃣ 创建表（如果不存在）
        unique_ptr<Statement> stmt(con->createStatement());
        stmt->execute("CREATE TABLE IF NOT EXISTS users ("
                      "id INT AUTO_INCREMENT PRIMARY KEY, "
                      "name VARCHAR(50), "
                      "email VARCHAR(100))");
        cout << "✅ Table 'users' ready." << endl;

        // 5️⃣ 插入数据
        unique_ptr<PreparedStatement> insertStmt(
            con->prepareStatement("INSERT INTO users (name, email) VALUES (?, ?)"));
        insertStmt->setString(1, "Alice");
        insertStmt->setString(2, "alice@example.com");
        insertStmt->execute();
        cout << "✅ Inserted 1 row." << endl;

        // 6️⃣ 查询数据
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT id, name, email FROM users"));
        cout << "\n📋 Current users:\n";
        while (res->next()) {
            cout << "ID: " << res->getInt("id")
                 << ", Name: " << res->getString("name")
                 << ", Email: " << res->getString("email") << endl;
        }

        // 7️⃣ 更新数据
        unique_ptr<PreparedStatement> updateStmt(
            con->prepareStatement("UPDATE users SET email=? WHERE name=?"));
        updateStmt->setString(1, "alice@newmail.com");
        updateStmt->setString(2, "Alice");
        updateStmt->execute();
        cout << "✅ Updated Alice's email." << endl;

        // 8️⃣ 删除数据
        unique_ptr<PreparedStatement> deleteStmt(
            con->prepareStatement("DELETE FROM users WHERE name=?"));
        deleteStmt->setString(1, "Alice");
        deleteStmt->execute();
        cout << "✅ Deleted Alice." << endl;

    } catch (sql::SQLException& e) {
        cerr << "❌ MySQL error: " << e.what() << endl;
        cerr << "Error Code: " << e.getErrorCode()
             << ", SQLState: " << e.getSQLState() << endl;
    }
    return 0;
}

