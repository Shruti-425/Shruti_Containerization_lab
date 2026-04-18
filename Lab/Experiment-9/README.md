
#  Experiment 9: Ansible Automation using Docker (SSH-based Multi-Server Setup)

##  Overview
This project demonstrates **configuration management and automation using Ansible** with multiple Docker containers acting as remote servers. It simulates a real-world infrastructure where Ansible manages multiple nodes via SSH.

---

##  Problem Statement
Manual server configuration leads to:
- Configuration drift
- Inconsistent environments
- Time-consuming repetitive tasks

This experiment solves these issues using **Ansible automation**.

---

##  Technologies Used
- Ubuntu (WSL2)
- Docker
- Ansible
- OpenSSH Server
- SSH (password authentication)
- Linux shell scripting

---

##  Architecture

```

WSL (Control Node - Ansible)
|
| SSH (2201 - 2204)
↓
Docker Containers (Managed Nodes)
├── server1
├── server2
├── server3
└── server4

````

---

##  Setup Steps

### 1. Create Docker Image
```bash
docker build --no-cache -t ubuntu-server .
````

---

### 2. Run 4 Containers (Servers)

```bash
for i in {1..4}; do
  docker run -d --rm -p 220${i}:22 --name server${i} ubuntu-server
done
```

---

### 3. Verify Running Containers

```bash
docker ps
```

---

### 4. SSH Test

```bash
ssh root@localhost -p 2201
```

Password:

```
password
```

---

##  Inventory File (inventory.ini)

```ini
[servers]
server1 ansible_host=localhost ansible_port=2201
server2 ansible_host=localhost ansible_port=2202
server3 ansible_host=localhost ansible_port=2203
server4 ansible_host=localhost ansible_port=2204

[servers:vars]
ansible_user=root
ansible_password=password
ansible_connection=ssh
ansible_python_interpreter=/usr/bin/python3
```

---

##  Test Connectivity

### Install dependency

```bash
sudo apt install sshpass -y
```

### Run Ansible Ping

```bash
ansible all -i inventory.ini -m ping
```

---

##  Expected Output

```
server1 | SUCCESS => pong
server2 | SUCCESS => pong
server3 | SUCCESS => pong
server4 | SUCCESS => pong
```

---

##  Sample Playbook (update.yml)

```yaml
---
- name: Configure servers
  hosts: servers
  become: yes

  tasks:
    - name: Update apt cache
      apt:
        update_cache: yes

    - name: Install packages
      apt:
        name:
          - vim
          - wget
          - htop
        state: present

    - name: Create test file
      copy:
        dest: /root/ansible_test.txt
        content: "Configured by Ansible on {{ inventory_hostname }}"
```

Run playbook:

```bash
ansible-playbook -i inventory.ini update.yml
```

---

##  Key Concepts Learned

* Ansible agentless architecture
* SSH-based automation
* Infrastructure as Code (IaC)
* Docker-based virtualization
* Inventory management

---

##  Common Issues & Fixes

### 1. SSH Permission Denied

* Ensure root password is set
* Enable PasswordAuthentication in sshd config

### 2. sshpass error

```bash
sudo apt install sshpass -y
```

### 3. Host key checking error

```bash
export ANSIBLE_HOST_KEY_CHECKING=False
```

---

##  Conclusion

This experiment successfully demonstrates how Ansible can automate configuration across multiple Docker-based servers using SSH, enabling scalable and consistent infrastructure management.

---