## Experiment 4: Docker Essentials

---

# Part 1: Containerizing Applications with Dockerfile

## Step 1: Create Project Folder

```bash
mkdir my-flask-app
cd my-flask-app
```
![Step 1 Output](IMAGES/img1.png)

---

## Step 2: Create Application Files

### app.py

```python
from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello():
    return "Hello from Docker!"

@app.route('/health')
def health():
    return "OK"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```

---

### requirements.txt

```
Flask==2.3.3
```

---

## Step 3: Create Dockerfile

```dockerfile
FROM python:3.10-slim

WORKDIR /app

COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

EXPOSE 5000

CMD ["python", "app.py"]
```

---

# Part 2: Using .dockerignore

Create `.dockerignore`

```
__pycache__/
*.pyc
*.pyo
*.pyd
.env
.venv
env/
venv/
.vscode/
.idea/
.git/
.gitignore
.DS_Store
Thumbs.db
*.log
logs/
tests/
test_*.py
```

---

# Part 3: Building Docker Images

## Step 1: Build Image

```bash
docker build -t my-flask-app .
```
![Step 1 Output](IMAGES/img2.png)

Check images:

```bash
docker images
```
![Step 1 Output](IMAGES/img3.png)

---

## Step 2: Tagging Images

Tag with version:

```bash
docker build -t my-flask-app:1.0 .
```

![Step 1 Output](IMAGES/img4.png)

Multiple tags:

```bash
docker build -t my-flask-app:latest -t my-flask-app:1.0 .
```
![Step 1 Output](IMAGES/img5.png)

Custom registry:

```bash
docker build -t ss1234ss1234/my-flask-app:1.0 .
```


Tag existing image:

```bash
docker tag my-flask-app:latest my-flask-app:v1
```
![Step 1 Output](IMAGES/img10.png)

---

## Step 3: View Image Details

```bash
docker images
docker history my-flask-app
docker inspect my-flask-app
```
![Step 1 Output](IMAGES/img11.png)
![Step 1 Output](IMAGES/img12.png)
---

# Part 4: Running Containers

## Step 1: Run Container

```bash
docker run -d -p 5000:5000 --name flask-container my-flask-app
```
![Step 1 Output](IMAGES/img13.png)

Test:

```bash
curl http://localhost:5000
```

Check:

```bash
docker ps
docker logs flask-container
```
![Step 1 Output](IMAGES/img14.png)
![Step 1 Output](IMAGES/img15.png)
---

## Step 2: Manage Containers

```bash
docker stop flask-container
docker start flask-container
docker rm flask-container
docker rm -f flask-container
```
![Step 1 Output](IMAGES/img16.png)

---

# Part 5: Multi-stage Builds

## Why Multi-stage?

* Smaller image size
* Better security
* Separate build & runtime

---

## Dockerfile.multistage

```dockerfile
FROM python:3.9-slim AS builder

WORKDIR /app

COPY requirements.txt .
RUN python -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
RUN pip install --no-cache-dir -r requirements.txt

FROM python:3.9-slim

WORKDIR /app

COPY --from=builder /opt/venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

COPY app.py .

RUN useradd -m -u 1000 appuser
USER appuser

EXPOSE 5000

CMD ["python", "app.py"]
```

---

## Build and Compare

```bash
docker build -t flask-regular .
docker build -f Dockerfile.multistage -t flask-multistage .
docker images | grep flask-
```
![Step 1 Output](IMAGES/img18.png)
![Step 1 Output](IMAGES/img19.png)

---

# Part 6: Publishing to Docker Hub

## Step 1: Login and Tag

```bash
docker login
docker tag my-flask-app:latest ss1234ss1234/my-flask-app:1.0
docker tag my-flask-app:latest ss1234ss1234/my-flask-app:latest
```
![Step 1 Output](IMAGES/img22.png)

Push:

```bash
docker push ss1234ss1234/my-flask-app:1.0
docker push ss1234ss1234/my-flask-app:latest
```
![Step 1 Output](IMAGES/img24.png)
![Step 1 Output](IMAGES/img25.png)
---

## Step 2: Pull & Run

```bash
docker pull ss1234ss1234/my-flask-app:latest
docker run -d -p 5000:5000 ss1234ss1234/my-flask-app:latest
```
![Step 1 Output](IMAGES/img26.png)
![Step 1 Output](IMAGES/img27.png)
---

# Part 7: Node.js Example

## Step 1: Create Node App

```bash
mkdir my-node-app
cd my-node-app
```
![Step 1 Output](IMAGES/img28.png)

### app.js

```javascript
const express = require('express');
const app = express();
const port = 3000;

app.get('/', (req, res) => {
  res.send('Hello from Node.js Docker!');
});

app.get('/health', (req, res) => {
  res.json({ status: 'healthy' });
});

app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
```

---

### package.json

```json
{
  "name": "node-docker-app",
  "version": "1.0.0",
  "main": "app.js",
  "dependencies": {
    "express": "^4.18.2"
  }
}
```

---

## Step 2: Node Dockerfile

```dockerfile
FROM node:18-alpine

WORKDIR /app

COPY package*.json ./
RUN npm install --only=production

COPY app.js .

EXPOSE 3000

CMD ["node", "app.js"]
```

---

## Step 3: Build & Run

```bash
docker build -t my-node-app .
docker run -d -p 3000:3000 --name node-container my-node-app
```
![Step 1 Output](IMAGES/img29.png)

Test:

```bash
curl http://localhost:3000
```

---

# Final Conclusion

This experiment covered:

* Dockerfile basics
* .dockerignore
* Image building & tagging
* Container management
* Multi-stage builds
* Docker Hub publishing
* Python & Node.js containerization