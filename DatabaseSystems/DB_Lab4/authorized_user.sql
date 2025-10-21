-- ==================================
-- (1) 创建用户
-- ==================================
-- 注意：'localhost' 表示用户只能从本机连接。如果需要远程连接，可以使用 '%'，但这有安全风险。
CREATE USER 'admin_user'@'localhost' IDENTIFIED BY 'adminNewSecureP@ssw0rd2025!';
CREATE USER 'manager_alice'@'localhost' IDENTIFIED BY 'managerNewSecureP@ssw0rd2025!';
CREATE USER 'reporter_bob'@'localhost' IDENTIFIED BY 'reporterNewSecureP@ssw0rd2025!';
CREATE USER 'csr_charlie'@'localhost' IDENTIFIED BY 'csrNewSecureP@ssw0rd2025!';

SELECT 'Users created.' 
AS Status;
FLUSH PRIVILEGES;

-- ==================================
-- (2) 表级权限授予与收回
-- ==================================

-- 授予 manager_alice 对 Orders 表的 SELECT, INSERT, UPDATE 权限
GRANT SELECT, INSERT, UPDATE ON order_management.Orders TO 'manager_alice'@'localhost';
SELECT "Granted SELECT, INSERT, UPDATE on Orders to manager_alice." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'manager_alice'@'localhost';

-- 授予 reporter_bob 对所有表的 SELECT 权限 (使用通配符)
GRANT SELECT ON order_management.* TO 'reporter_bob'@'localhost';
SELECT "Granted SELECT on all tables in order_management to reporter_bob." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'reporter_bob'@'localhost';

-- 授予 admin_user 对 Customer 表的所有权限
GRANT ALL PRIVILEGES ON order_management.Customer TO 'admin_user'@'localhost';
SELECT "Granted ALL PRIVILEGES on Customer to admin_user." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'admin_user'@'localhost';

-- ----------------------------------
-- 收回权限
-- ----------------------------------
-- 收回 manager_alice 对 Orders 表的 UPDATE 权限
REVOKE UPDATE ON order_management.Orders FROM 'manager_alice'@'localhost';
SELECT "Revoked UPDATE on Orders from manager_alice." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'manager_alice'@'localhost'; 

-- 收回 admin_user 对 Customer 表的 DELETE 权限 (之前授予了 ALL)
REVOKE DELETE ON order_management.Customer FROM 'admin_user'@'localhost';
SELECT "Revoked DELETE on Customer from admin_user." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'admin_user'@'localhost'; 

-- ==================================
-- (3) 列级权限授予与收回
-- ==================================

-- 授予 csr_charlie 对 Customer 表的 name 和 phone 列的 SELECT 权限
GRANT SELECT (name, phone) ON order_management.Customer TO 'csr_charlie'@'localhost';
SELECT "Granted SELECT(name, phone) on Customer to csr_charlie." AS Status;

-- 授予 csr_charlie 对 Customer 表的 phone 列的 UPDATE 权限
GRANT UPDATE (phone) ON order_management.Customer TO 'csr_charlie'@'localhost';
SELECT "Granted UPDATE(phone) on Customer to csr_charlie." AS Status;

FLUSH PRIVILEGES;
SHOW GRANTS FOR 'csr_charlie'@'localhost';

-- ----------------------------------
-- 收回列权限
-- ----------------------------------
-- 收回 csr_charlie 对 Customer 表 phone 列的 UPDATE 权限
REVOKE UPDATE (phone) ON order_management.Customer FROM 'csr_charlie'@'localhost';
SELECT "Revoked UPDATE(phone) on Customer from csr_charlie." AS Status;
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'csr_charlie'@'localhost'; -- 应不再有 UPDATE(phone) on Customer

-- ==================================
-- (4) 角色创建、授权与收回
-- ==================================

-- 创建角色
CREATE ROLE 'OrderManagerRole';
SELECT "Role created: OrderManagerRole." AS Status;

-- 为角色授权：管理订单、订单项、支付，查看客户和菜单
GRANT SELECT, INSERT, UPDATE, DELETE ON order_management.Orders TO 'OrderManagerRole';
GRANT SELECT, INSERT, UPDATE, DELETE ON order_management.OrderItem TO 'OrderManagerRole';
GRANT SELECT, INSERT, UPDATE, DELETE ON order_management.Payment TO 'OrderManagerRole';
GRANT SELECT ON order_management.Customer TO 'OrderManagerRole';
GRANT SELECT ON order_management.Menu TO 'OrderManagerRole';
SELECT "Granted privileges to OrderManagerRole." AS Status;

FLUSH PRIVILEGES;

-- ----------------------------------
-- 将角色授予用户
-- ----------------------------------
GRANT 'OrderManagerRole' TO 'manager_alice'@'localhost';
SELECT "Granted OrderManagerRole to manager_alice." AS Status;

-- 设置默认角色（登录自动激活）
SET DEFAULT ROLE 'OrderManagerRole' TO 'manager_alice'@'localhost';
SELECT "Set default role for manager_alice." AS Status;

FLUSH PRIVILEGES;

-- 查看角色和对应用户拥有的权限
SHOW GRANTS FOR 'OrderManagerRole';
SHOW GRANTS FOR 'manager_alice'@'localhost';

-- ----------------------------------
-- 收回用户的角色
-- ----------------------------------
REVOKE 'OrderManagerRole' FROM 'manager_alice'@'localhost';
FLUSH PRIVILEGES;

SELECT "Script finished." AS Status;