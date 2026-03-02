#  Experiment 5: Docker – Volumes, Environment Variables, Monitoring & Networks

---

# Part 1: Docker Volumes – Persistent Data Storage

## Understanding Data Persistence

### The Problem: Container Data is Ephemeral

Create a container and write data:

```bash
docker run -it --name test-container ubuntu /bin/bash
```

Inside container:

```bash
echo "Hello World" > /data/message.txt
cat /data/message.txt
exit
```

Restart container:

```bash
docker start test-container
docker exec test-container cat /data/message.txt
```

## Lab 2: Volume Types

###  Anonymous Volumes

```bash
docker run -d -v /app/data --name web1 nginx
docker volume ls
docker inspect web1 | grep -A 5 Mounts
```

---

###  Named Volumes

```bash
docker volume create mydata
docker run -d -v mydata:/app/data --name web2 nginx
docker volume ls
docker volume inspect mydata
```

---

### Bind Mounts (Host Directory)

```bash
mkdir ~/myapp-data

docker run -d -v ~/myapp-data:/app/data --name web3 nginx

echo "From Host" > ~/myapp-data/host-file.txt

docker exec web3 cat /app/data/host-file.txt
```

✔ Data from host is visible inside container.

---

## Lab 3: Practical Volume Examples

### Example 1: MySQL with Persistent Storage

```bash
docker run -d \
  --name mysql-db \
  -v mysql-data:/var/lib/mysql \
  -e MYSQL_ROOT_PASSWORD=itssecret \
  mysql:8.0
```

Remove and recreate:

```bash
docker stop mysql-db
docker rm mysql-db

docker run -d \
  --name new-mysql \
  -v mysql-data:/var/lib/mysql \
  -e MYSQL_ROOT_PASSWORD=secret \
  mysql:8.0
```

✔ Data is preserved using named volume.

---

### Example 2: Nginx Config with Bind Mount

```bash
mkdir ~/nginx-config
```

Create config:

```bash
echo 'server {
    listen 80;
    server_name localhost;
    location / {
        return 200 "Hello from mounted config!";
    }
}' > ~/nginx-config/nginx.conf
```

Run container:

```bash
docker run -d \
  --name nginx-custom \
  -p 8080:80 \
  -v ~/nginx-config/nginx.conf:/etc/nginx/conf.d/default.conf \
  nginx
```

Test:

```bash
curl http://localhost:8080
```

---

## Lab 4: Volume Management Commands

```bash
docker volume ls
docker volume create app-volume
docker volume inspect app-volume
docker volume prune
docker volume rm volume-name
docker cp local-file.txt container-name:/path/in/volume
```

---

# Part 2: Environment Variables

## Lab 1: Setting Environment Variables

### Method 1: Using -e Flag

```bash
docker run -d \
  --name app1 \
  -e DATABASE_URL="postgres://user:pass@db:5432/mydb" \
  -e DEBUG="true" \
  -p 3000:3000 \
  my-node-app
```

Multiple variables:

```bash
docker run -d \
  -e VAR1=value1 \
  -e VAR2=value2 \
  -e VAR3=value3 \
  my-node-app
```

---

### Method 2: Using --env-file

Create `.env`:

```bash
echo "DATABASE_HOST=localhost" > .env
echo "DATABASE_PORT=5432" >> .env
echo "API_KEY=itssecret123" >> .env
```

Run:

```bash
docker run -d --env-file .env --name app2 my-app
```

---

### Method 3: In Dockerfile

```dockerfile
ENV NODE_ENV=production
ENV PORT=3000
ENV APP_VERSION=1.0.0
```

---

## Lab 2: Flask Example with Environment Variables

### app.py

```python
import os
from flask import Flask

app = Flask(__name__)

db_host = os.environ.get('DATABASE_HOST', 'localhost')
debug_mode = os.environ.get('DEBUG', 'false').lower() == 'true'
api_key = os.environ.get('API_KEY')

@app.route('/config')
def config():
    return {
        'db_host': db_host,
        'debug': debug_mode,
        'has_api_key': bool(api_key)
    }

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    app.run(host='0.0.0.0', port=port, debug=debug_mode)
```

---

## Lab 3: Test Environment Variables

```bash
docker run -d \
  --name flask-app \
  -p 5001:8080 \
  -e DATABASE_HOST="prod-db.example.com" \
  -e DEBUG="true" \
  -e PORT="8080" \
  flask-app
```

Check inside container:

```bash
docker exec flask-app env
docker exec flask-app printenv DATABASE_HOST
```

Test endpoint
---

# Part 3: Docker Monitoring

## Lab 1: docker stats

```bash
docker stats
docker stats --no-stream
docker stats container1 container2
```

---

## Lab 2: docker top

```bash
docker top container-name
docker top container-name -ef
```

---

## Lab 3: docker logs

```bash
docker logs container-name
docker logs -f container-name
docker logs --tail 100 container-name
docker logs -t container-name
```

---

## Lab 4: docker inspect

```bash
docker inspect container-name
docker inspect --format='{{.State.Status}}' container-name
docker inspect --format='{{.NetworkSettings.IPAddress}}' container-name
```

---

## Lab 5: docker events

```bash
docker events
docker events --filter 'type=container'
docker events --filter 'event=start'
```

---

# Part 4: Docker Networks

## Lab 1: List Networks

```bash
docker network ls
```

---

## Lab 2: Bridge Network

```bash
docker network create my-network

docker run -d --name web1 --network my-network nginx
docker run -d --name web2 --network my-network nginx

docker exec web1 curl http://web2
```

---

## Host Network

```bash
docker run -d --name host-app --network host nginx
```

---

## None Network

```bash
docker run -d --name isolated-app --network none alpine sleep 3600
```

---

## Overlay Network (Swarm)

```bash
docker network create --driver overlay my-overlay
```

---

# Part 5: Complete Real-World Example

## Architecture

* Flask Web App (5000)
* PostgreSQL (5432)
* Redis (6379)
* Custom Docker network

---

## Implementation

### Create Network

```bash
docker network create myapp-network
```

### Start PostgreSQL

```bash
docker run -d \
  --name postgres \
  --network myapp-network \
  -e POSTGRES_PASSWORD=mysecretpassword \
  -e POSTGRES_DB=mydatabase \
  -v postgres-data:/var/lib/postgresql/data \
  postgres:15
```

---

### Start Redis

```bash
docker run -d \
  --name redis \
  --network myapp-network \
  -v redis-data:/data \
  redis:7-alpine
```

---

### Start Flask App

```bash
docker run -d \
  --name flask-app \
  --network myapp-network \
  -p 5000:5000 \
  -v $(pwd)/app:/app \
  -v app-logs:/var/log/app \
  -e DATABASE_URL="postgresql://postgres:mysecretpassword@postgres:5432/mydatabase" \
  -e REDIS_URL="redis://redis:6379" \
  -e DEBUG="false" \
  flask-app:latest
```

---

## Monitoring Commands

```bash
docker ps
docker stats postgres redis flask-app
docker logs -f flask-app
docker exec flask-app ping -c 2 postgres
docker exec flask-app ping -c 2 redis
docker network inspect myapp-network
```

---

# Final Conclusion

This experiment covered:

* Docker volumes (anonymous, named, bind mounts)
* Data persistence
* Environment variables in Docker
* Monitoring containers
* Docker networking concepts