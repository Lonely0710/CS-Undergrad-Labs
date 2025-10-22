import smtplib
from email.mime.text import MIMEText
from email.header import Header

# 发件人信息
sender = 'you@example.com'  # 发件人邮箱
sender_password = 'EMAIL_PASSWORD'  # 发件人邮箱密码或授权码

# 收件人信息
receivers = ['smtp.example.com']  # 收件人邮箱，可以设置多个收件人

# 邮件内容
subject = 'Python SMTP 邮件测试'
content = '这是一封使用 Python SMTP 发送的测试邮件。'

# 构建邮件对象
message = MIMEText(content, 'plain', 'utf-8')
message['From'] = Header(sender)
message['To'] = Header(','.join(receivers))
message['Subject'] = Header(subject, 'utf-8')

try:
    # 连接 SMTP 服务器
    smtp_obj = smtplib.SMTP_SSL('smtp.qq.com', 465)  # 使用 SSL 加密，端口 465
    smtp_obj.login(sender, sender_password)

    # 发送邮件
    smtp_obj.sendmail(sender, receivers, message.as_string())
    print('邮件发送成功')
except smtplib.SMTPException as e:
    print('邮件发送失败:', e)
finally:
    if 'smtp_obj' in locals():
      smtp_obj.quit()#关闭smtp服务。