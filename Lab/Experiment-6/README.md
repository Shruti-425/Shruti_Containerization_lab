#  Experiment 6: Docker Run vs Docker Compose & Multi-Container Applications

---

##  Objective

* Understand the difference between **Docker Run (imperative)** and **Docker Compose (declarative)**
* Learn how to convert `docker run` commands into `docker-compose.yml`
* Deploy **single-container** and **multi-container applications**
* Work with **networks, volumes, and environment variables**
* Build custom images using **Dockerfile**
* Explore **scaling and orchestration concepts**

---

#  Part A – Theory

##  Docker Run (Imperative Approach)

* Uses CLI commands to run containers
* Requires manual flags for configuration:

  * Port mapping (`-p`)
  * Volumes (`-v`)
  * Environment variables (`-e`)
  * Network (`--network`)
  * Restart policies (`--restart`)
  * Resource limits (`--memory`, `--cpus`)

###  Example

```bash
docker run -d \
  --name my-nginx \
  -p 8080:80 \
  -v ./html:/usr/share/nginx/html \
  -e NGINX_HOST=localhost \
  --restart unless-stopped \
  nginx:alpine
```

---

##  Docker Compose (Declarative Approach)

* Uses `docker-compose.yml`
* Defines desired system state
* One command to manage everything

###  Example

```yaml
version: '3.8'
services:
  nginx:
    image: nginx:alpine
    container_name: my-nginx
    ports:
      - "8080:80"
    volumes:
      - ./html:/usr/share/nginx/html
    environment:
      NGINX_HOST: localhost
    restart: unless-stopped
```

###  Run

```bash
docker compose up -d
```

---

##  Mapping: Docker Run vs Compose

| Docker Run | Docker Compose                 |
| ---------- | ------------------------------ |
| -p         | ports                          |
| -v         | volumes                        |
| -e         | environment                    |
| --name     | container_name                 |
| --network  | networks                       |
| --restart  | restart                        |
| --memory   | deploy.resources.limits.memory |
| --cpus     | deploy.resources.limits.cpus   |

---

##  Advantages of Docker Compose

* Multi-container support
* Easy reproducibility
* Version control friendly
* Simplified lifecycle management
* Supports scaling

---

#  Part B – Practical Tasks

---

## 🔹 Task 1: Single Container (Nginx)

###   Using Docker Run

```bash
docker run -d \
  --name lab-nginx \
  -p 8081:80 \
  -v $(pwd)/html:/usr/share/nginx/html \
  nginx:alpine
```

###  Access

```
http://localhost:8081
```

###  Stop

```bash
docker stop lab-nginx
docker rm lab-nginx
```

---

###  Using Docker Compose

```yaml
version: '3.8'
services:
  nginx:
    image: nginx:alpine
    container_name: lab-nginx
    ports:
      - "8081:80"
    volumes:
      - ./html:/usr/share/nginx/html
```

###  Run

```bash
docker compose up -d
```

###  Stop

```bash
docker compose down
```

---

##  Task 2: Multi-Container App (WordPress + MySQL)

---

###  Using Docker Run

```bash
docker network create wp-net

docker run -d \
  --name mysql \
  --network wp-net \
  -e MYSQL_ROOT_PASSWORD=secret \
  -e MYSQL_DATABASE=wordpress \
  mysql:5.7

docker run -d \
  --name wordpress \
  --network wp-net \
  -p 8082:80 \
  -e WORDPRESS_DB_HOST=mysql \
  -e WORDPRESS_DB_PASSWORD=secret \
  wordpress:latest
```

###  Access

```
http://localhost:8082
```

---

###  Using Docker Compose

```yaml
version: '3.8'
services:
  mysql:
    image: mysql:5.7
    environment:
      MYSQL_ROOT_PASSWORD: secret
      MYSQL_DATABASE: wordpress
    volumes:
      - mysql_data:/var/lib/mysql

  wordpress:
    image: wordpress:latest
    ports:
      - "8082:80"
    environment:
      WORDPRESS_DB_HOST: mysql
      WORDPRESS_DB_PASSWORD: secret
    depends_on:
      - mysql

volumes:
  mysql_data:
```

###  Run

```bash
docker compose up -d
```

###  Stop

```bash
docker compose down -v
```

---

#  Part C – Conversion Tasks

---

##  Task 3: Convert Docker Run → Compose

### Example Output

```yaml
version: '3.8'
services:
  webapp:
    image: node:18-alpine
    container_name: webapp
    ports:
      - "5000:5000"
    environment:
      APP_ENV: production
      DEBUG: "false"
    restart: unless-stopped
```

---

##  Task 4: Resource Limits

```yaml
version: '3.8'
services:
  limited-app:
    image: nginx:alpine
    ports:
      - "9000:9000"
    restart: always
    deploy:
      resources:
        limits:
          memory: 256M
          cpus: "0.5"
```

###  Note:

* `deploy` works **only in Docker Swarm**
* Ignored in normal Docker Compose

---

#  Part D – Dockerfile + Compose

---

##  Task 5: Node App with Dockerfile

###  app.js

```js
const http = require('http');
http.createServer((req, res) => {
  res.end("Docker Compose Build Lab");
}).listen(3000);
```

---

###  Dockerfile

```dockerfile
FROM node:18-alpine
WORKDIR /app
COPY app.js .
EXPOSE 3000
CMD ["node", "app.js"]
```

---

###  docker-compose.yml

```yaml
version: '3.8'
services:
  nodeapp:
    build:
      context: .
      dockerfile: Dockerfile
    container_name: custom-node-app
    ports:
      - "3000:3000"
```

### ▶ Run

```bash
docker compose up --build -d
```

###  Access

```
http://localhost:3000
```

---

# 🅱 Experiment 6B – WordPress + MySQL (Compose)

---

##  Architecture

```
Browser → WordPress → MySQL → Volume Storage
```

---

##  docker-compose.yml

```yaml
version: '3.9'
services:
  db:
    image: mysql:5.7
    container_name: wordpress_db
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: rootpass
      MYSQL_DATABASE: wordpress
      MYSQL_USER: wpuser
      MYSQL_PASSWORD: wppass
    volumes:
      - db_data:/var/lib/mysql

  wordpress:
    image: wordpress:latest
    container_name: wordpress_app
    depends_on:
      - db
    ports:
      - "8080:80"
    restart: always
    environment:
      WORDPRESS_DB_HOST: db:3306
      WORDPRESS_DB_USER: wpuser
      WORDPRESS_DB_PASSWORD: wppass
      WORDPRESS_DB_NAME: wordpress
    volumes:
      - wp_data:/var/www/html

volumes:
  db_data:
  wp_data:
```

---

##  Run

```bash
docker compose up -d
```

##  Access

```
http://localhost:8080
```

---

##  Verify

```bash
docker ps
docker volume ls
```

---

##  Stop

```bash
docker compose down
```

---

#  Scaling

```bash
docker compose up --scale wordpress=3
```

 Limitation:

* No load balancing
* Same port conflict
* Use **Nginx reverse proxy** for scaling

---

#  Docker Compose vs Docker Swarm

| Feature        | Compose | Swarm    |
| -------------- | ------- | -------- |
| Scaling        | Limited | Advanced |
| Load Balancing | ❌      | ✅      |
| Self Healing   | ❌      | ✅      |
| Multi-node     | ❌      | ✅      |

---

#  Learning Outcomes

* Difference between **imperative vs declarative**
* Multi-container deployment
* Networking & volumes
* Dockerfile integration
* Scaling limitations
* Intro to orchestration (Swarm)

---

#  Conclusion

This experiment demonstrates:

* Docker Run vs Docker Compose comparison
* Efficient container orchestration using Compose
* Real-world deployment of WordPress + MySQL
* Importance of volumes and networking
* Limitations of Compose for production scaling

---
