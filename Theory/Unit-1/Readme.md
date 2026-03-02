# Install & Enable WSL (Ubuntu)

```bash
wsl --install -d ubuntu
```

Restart your system after installation.

---

#  Install Docker Engine on Ubuntu (WSL)

```bash
sudo apt update
sudo apt install docker.io
```

### Verify Installation

```bash
docker -v
docker run hello-world
```

---

# Docker Basic Commands

### Check Docker Version

```bash
sudo docker version
```

### List Local Images

```bash
docker images
```

### Pull Image from Registry

```bash
docker pull ubuntu
```

### Remove Image

```bash
docker rmi ubuntu
```

### Run Container

```bash
docker run ubuntu
docker run -it --name test ubuntu bash
```

### List Running Containers

```bash
docker ps
```

---

#  Preserve Changes Inside a Container

### Step 1: Run Base Ubuntu Container

```bash
docker run -it --name java_lab ubuntu:22.04
```

### Step 2: Install Java

```bash
apt update
apt install -y openjdk-17-jdk
javac --version
```

### Step 3: Create Java Application

```bash
mkdir -p /home/app
cd /home/app
apt install -y nano
nano HelloWorld.java
```

Example Java Code:

```java
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello from Docker!");
    }
}
```

### Step 4: Compile & Run

```bash
javac HelloWorld.java
java HelloWorld
```

### Step 5: Exit Container

```bash
exit
```

---

#  Convert Container to Image

```bash
docker commit java_lab ss1234ss1234/java-app:1.0
```

### Run the Exported Image

```bash
docker run -it ss1234ss1234/java-app:1.0 bash
```

---

#  Save & Load Image (Offline Transfer)

### Save Image

```bash
docker save -o java-app.tar ss1234ss1234/java-app:1.0
```

### Load Image

```bash
docker load -i java-app.tar
```

---

#  Create Dockerfile for Java Application

### Create Dockerfile

```dockerfile
FROM ubuntu:22.04

RUN apt update && apt install -y openjdk-17-jdk

WORKDIR /app

COPY HelloWorld.java .

RUN javac HelloWorld.java

CMD ["java", "HelloWorld"]
```

---

#  Build Image from Dockerfile

```bash
docker build -t ss1234ss1234/java-app:1.0 .
docker images
```

---

#  Run Container from Built Image

```bash
docker run ss1234ss1234/java-app:1.0
```

---

# Modify CMD (Second Dockerfile Example)

```dockerfile
CMD ["echo","Hello from version 2"]
```

Rebuild image:

```bash
docker build -t ss1234ss1234/java-app:2.0 .
```

---

# Push Image to Docker Hub
## Tag

```bash
docker tag java-app:1.0 ss1234ss1234/java-app:1.0
```

### Push to Registry

```bash
docker push ss1234ss1234/java-app:1.0
```

---

# Pull Image

```bash
docker pull ss1234ss1234/java-app:1.0
docker run ss1234ss1234/java-app:1.0
```

---

# Docker Engine API

---

## Verify Docker API Socket

```bash
ls -l /var/run/docker.sock
```

---

## Check API Version

```bash
curl --unix-socket /var/run/docker.sock http://localhost/version
```

---

## List Containers via API

```bash
curl --unix-socket /var/run/docker.sock \
  http://localhost/v1.44/containers/json
```

---

## Pull Nginx Using API

```bash
curl --unix-socket /var/run/docker.sock \
  -X POST \
  "http://localhost/v1.44/images/create?fromImage=nginx&tag=latest"
```

---

## Start / Stop Container Using API

```bash
curl --unix-socket /var/run/docker.sock \
  -X POST \
  http://localhost/v1.44/containers/mynginx/start

curl --unix-socket /var/run/docker.sock \
  -X POST \
  http://localhost/v1.44/containers/mynginx/stop
```

---

# Expose Docker Remote API

### Docker Daemon Configuration

```bash
sudo nano /etc/docker/daemon.json
```

Add:

```json
{
  "hosts": [
    "tcp://0.0.0.0:2375",
    "unix:///var/run/docker.sock"
  ]
}
```

---

### Stop Docker Daemon

```bash
sudo pkill dockerd
```

### Start Docker Daemon Manually

```bash
sudo dockerd &sudo dockerd &
```

---

### Verify Docker is Listening on Port 

```bash
ss -lntp | grep 2375
```

---