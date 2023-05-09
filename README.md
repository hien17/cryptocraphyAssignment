# Attack

Teardrop
  • Sử dụng hệ điều hành Ubuntu và Mininet-wifi 
  • Dùng lệnh sudo mn để khởi động hệ thống
  • Dùng lệnh xterm h1 để mở console của máy h1 và tương tự xterm h2 đối với h2.
  • Ta có thể kiểm tra địa chỉ IP của máy h1 là 10.0.0.1 và h2 là 10.0.02 qua lệnh ip a.
  • Ta chạy lệnh python teardrop.py 10.0.0.2 0 ở h1 để thực hiện gửi các gói dữ liệu tới h2.
  Trong đó file teardrop.py là file hiện thực việc phương thức tấn công tear drop
  • Ta có thể thấy sự sai khác trong fragment offset và data size
  
Ping of Death
• Sử dụng hệ điều hành Ubuntu và Mininet-wifi
• Dùng lệnh sudo mn để khởi động hệ thống
• Dùng lệnh xterm h1 để mở console của máy h1 và tương tự xterm h2 đối với h2.
• Ta có thể kiểm tra địa chỉ IP của máy h1 là 192.168.4.73 và h2 là 192.168.4.75 qua lệnh
ip a.
• Ta chạy lệnh python ping.py ở h1 để thực hiện gửi các gói dữ liệu tới h2.
• Ta có thể thấy sự sai khác trong fragment offset và data size

TCP SYN Flood
hiện thực tấn công TCP SYN flood bằng Kali Linux với hping3.
Dòng bên dưới cho phép ta bắt đầu hướng cuộc tấn công SYN flood tới địa chỉ IP của nạn
nhân (192.168.100.5):
Figure 14: TCP synflood attack
Sau đó, ta bật Wireshark ở máy nạn nhân để phát hiện cuộc tấn công

DNS Amplification
Hiện thực kiểu tấn công DNS Amplification trong file DAA.py

# Defend

Ta có thể sử dụng iptables cùng với snort để phát hiện cái mối đe dọa đến hệ thống
  • Cập nhật hệ thống
    sudo apt update

  • Cài đặt iptables
    sudo apt install

  • Nhập các lệnh sau để giúp iptables phát hiện ra lưu lượng bất thường và drop các gói đe dọa
    sudo iptables -A INPUT -p tcp -- syn -m limit -- limit
    s -- limit - burst 3 -j RETURN
    sudo iptables -N syn_flood
    sudo iptables -A INPUT -p tcp -- syn -j syn_flood
    sudo iptables -A syn_flood -m limit -- limit 1/ s --
    limit - burst 3 -j RETURN
    sudo iptables -A syn_flood -j DROP

  • Tiếp đến là cài đặt snort
    sudo apt install snort -y
    
  • Mở file config và chỉnh sửa HOME_NET any thành địa chỉ ip của máy
    sudo gedit / etc / snort / snort . conf

  • Thêm rules mới ở file local.rules, ta có thể thêm rules khác để phòng tránh các kiểu tấn
công DDoS khác.
    sudo gedit / etc / snort / rules / local . rules
    alert tcp any any -> any any ( flags : S ;
    detection_filter : track by_src , count 10 , seconds 1;
    msg : " Possible SYN Flood "; sid : 1000001;)

  • Khởi động lại snort, các mục tiêu đe dọa sẽ được phát hiện và in ra trên console
    asudo snort -c / etc / snort / snort . conf -A console
