#  Experiment 10: SonarQube — Static Code Analysis

##  Overview

This project demonstrates how to use **SonarQube** for static code analysis using Docker. It includes:

* SonarQube Server (analysis + dashboard)
* PostgreSQL Database (data storage)
* Sample Java application with intentional issues

---

##  Problem Statement

Code bugs and security issues are often discovered too late. Manual code reviews:

* Are slow
* Do not scale
* Are inconsistent

 **Solution:** Use SonarQube to automatically detect:

* Bugs
* Vulnerabilities
* Code Smells
* Technical Debt

---

##  Architecture

```
Your Code → Sonar Scanner → SonarQube Server → PostgreSQL DB → Dashboard
```

### Components:

* **SonarQube Server** → Stores results, applies rules, shows dashboard
* **Sonar Scanner** → Analyzes code and sends report
* **PostgreSQL** → Stores analysis data

---

##  Prerequisites

* Docker
* Docker Compose
* Java (JDK 11+)
* Maven

---

##  Step 1: Setup SonarQube using Docker

Create `docker-compose.yml`:

```yaml
services:
  sonar-db:
    image: postgres:13
    container_name: sonar-db
    environment:
      POSTGRES_USER: sonar
      POSTGRES_PASSWORD: sonar
      POSTGRES_DB: sonarqube
      POSTGRES_HOST_AUTH_METHOD: trust
    volumes:
      - sonar-db-data:/var/lib/postgresql/data
    networks:
      - sonarqube-lab

  sonarqube:
    image: sonarqube:lts-community
    container_name: sonarqube
    ports:
      - "9000:9000"
    environment:
      SONAR_JDBC_URL: jdbc:postgresql://sonar-db:5432/sonarqube
      SONAR_JDBC_USERNAME: sonar
      SONAR_JDBC_PASSWORD: sonar
    volumes:
      - sonar-data:/opt/sonarqube/data
      - sonar-extensions:/opt/sonarqube/extensions
    depends_on:
      - sonar-db
    networks:
      - sonarqube-lab

volumes:
  sonar-db-data:
  sonar-data:
  sonar-extensions:

networks:
  sonarqube-lab:
```

###  Run Containers

```bash
docker-compose up -d
docker-compose logs -f sonarqube
```

 Open in browser:
http://localhost:9000



---

##  Step 2: Generate Token

1. Go to **My Account → Security**
2. Generate token (e.g., `scanner-token`)
3. Copy it (shown only once)

---

##  Step 3: Sample Java Project

### Project Structure

```
sample-java-app/
 └── src/main/java/com/example/Calculator.java
 └── pom.xml
```

### Features with Issues:

* Division by zero bug
* SQL Injection vulnerability
* Null pointer risk
* Duplicate code
* Unused variables

---

##  Step 4: Maven Configuration

Add in `pom.xml`:

```xml
<properties>
    <sonar.projectKey>sample-java-app</sonar.projectKey>
    <sonar.host.url>http://localhost:9000</sonar.host.url>
    <sonar.login>YOUR_TOKEN</sonar.login>
</properties>
```

---

## ▶ Step 5: Run SonarQube Analysis

### Using Maven

```bash
mvn sonar:sonar -Dsonar.login=YOUR_TOKEN
```

---

##  Step 6: View Dashboard

Open:

```
http://localhost:9000
```

---

## 🔍 API Example

```bash
curl -u admin:YOUR_TOKEN \
"http://localhost:9000/api/issues/search?projectKeys=sample-java-app&types=BUG"
```

---

##  CI/CD Integration (Jenkins)

Pipeline Flow:

```
Checkout → Analyze → Quality Gate → Build → Deploy
```

Example snippet:

```groovy
stage('SonarQube Analysis') {
    steps {
        withSonarQubeEnv('SonarQube') {
            sh 'mvn clean verify sonar:sonar'
        }
    }
}
```

---

##  Key Concepts

| Term           | Meaning              |
| -------------- | -------------------- |
| Bug            | Code that may fail   |
| Vulnerability  | Security risk        |
| Code Smell     | Poor design          |
| Technical Debt | Effort to fix issues |
| Quality Gate   | Pass/Fail criteria   |

---

## 🏁 Conclusion

* SonarQube helps maintain high code quality
* Scanner + Server both are required
* Enables automated code review in CI/CD

---