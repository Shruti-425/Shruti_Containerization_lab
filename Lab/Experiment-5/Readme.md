#  Experiment 5: Docker - Volumes, Environment Variables, Monitoring & Networks

---

##  Objective

To understand advanced Docker concepts including:

 Data persistence using volumes
 Managing environment variables
 Monitoring containers
 Docker networking and multi-container communication

---

#  Part 1: Docker Volumes – Persistent Data Storage

##  Problem: Ephemeral Data

By default, container data is **temporary**. When a container stops or is removed, data is lost.

```bash
docker run -it --name test-container ubuntu /bin/bash

echo "Hello World" > /data/message.txt
cat /data/message.txt
exit

docker start test-container
docker exec test-container cat /data/message.txt
```

 Data is lost after restart.

---

##  Volume Types

### 1. Anonymous Volumes

```bash
docker run -d -v /app/data --name web1 nginx
docker volume ls
docker inspect web1
```

 Auto-created volume with random name

---

### 2. Named Volumes

```bash
docker volume create mydata
docker run -d -v mydata:/app/data --name web2 nginx

docker volume ls
docker volume inspect mydata
```

 Persistent and reusable

---

### 3. Bind Mounts (Host Directory)

```bash
mkdir ~/myapp-data
docker run -d -v ~/myapp-data:/app/data --name web3 nginx

echo "From Host" > ~/myapp-data/host-file.txt
docker exec web3 cat /app/data/host-file.txt
```

 Direct link between host and container

---

##  Practical Example: MySQL Persistence

```bash
docker run -d \
--name mysql-db \
-v mysql-data:/var/lib/mysql \
-e MYSQL_ROOT_PASSWORD=secret \
mysql:8.0
```

 Data persists even after container removal.

---

#  Part 2: Environment Variables

##  Method 1: Using -e

```bash
docker run -d \
--name app1 \
-e DEBUG=true \
-e PORT=3000 \
my-node-app
```

---

##  Method 2: Using .env File

```bash
echo "DATABASE_HOST=localhost" > .env

docker run -d \
--env-file .env \
--name app2 \
my-app
```

---

##  Method 3: In Dockerfile

```dockerfile
ENV NODE_ENV=production
ENV PORT=3000
```

---

##  Flask Example

Application reads environment variables:

```python
import os
db_host = os.environ.get('DATABASE_HOST', 'localhost')
```

---

##  Testing

```bash
docker exec flask-app env
curl http://localhost:5001/config
```

---

#  Part 3: Docker Monitoring

##  Real-time Stats

```bash
docker stats
docker stats --no-stream
```

---

##  Logs

```bash
docker logs container-name
docker logs -f container-name
```

---

##  Processes

```bash
docker top container-name
```

---

##  Inspect Container

```bash
docker inspect container-name
```

---

##  Events Monitoring

```bash
docker events
docker events --filter 'event=start'
```

---

#  Part 4: Docker Networks

##  List Networks

```bash
docker network ls
```

---

##  Bridge Network

```bash
docker network create my-network

docker run -d --name web1 --network my-network nginx
docker run -d --name web2 --network my-network nginx

docker exec web1 curl http://web2
```

 Containers communicate using names

---

##  Host Network

```bash
docker run -d --network host nginx
```

 Uses host network directly

---

## 🔹 None Network

```bash
docker run -d --network none alpine sleep 3600
```

 No internet access

---

##  Network Management

```bash
docker network create app-network
docker network connect app-network container-name
docker network rm network-name
```

---

#  Part 5: Multi-Container Application

##  Architecture

* Flask App
* PostgreSQL Database
* Redis Cache

---

##  Implementation

```bash
docker network create myapp-network

docker run -d \
--name postgres \
--network myapp-network \
-e POSTGRES_PASSWORD=mysecretpassword \
-v postgres-data:/var/lib/postgresql/data \
postgres:15

docker run -d \
--name redis \
--network myapp-network \
redis:7-alpine

docker run -d \
--name flask-app \
--network myapp-network \
-p 5000:5000 \
-e DATABASE_URL="postgresql://postgres:mysecretpassword@postgres:5432/mydatabase" \
-e REDIS_URL="redis://redis:6379" \
flask-app
```

---

##  Monitoring

```bash
docker ps
docker stats
docker logs -f flask-app
```

---

##  Connectivity Test

```bash
docker exec flask-app ping postgres
docker exec flask-app ping redis
```

---

#  Key Concepts Learned

* Docker volumes enable **persistent storage**
* Environment variables allow **dynamic configuration**
* Monitoring tools help track **performance & logs**
* Docker networks enable **container communication**
* Multi-container apps simulate **real-world architecture**

---

#  Conclusion

This experiment demonstrates how Docker supports:

* Persistent storage
* Configurable environments
* Monitoring and debugging
* Scalable multi-container systems

---