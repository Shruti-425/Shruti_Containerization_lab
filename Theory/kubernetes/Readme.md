#   Kubernetes Hands-on: Apache Web Server Deployment (httpd)

##  Objective

This project demonstrates how to deploy, manage, scale, and debug a simple Apache web server (`httpd`) using Kubernetes.

By completing this lab, you will learn:

* How to run a Pod
* Convert a Pod into a Deployment
* Expose applications using Services
* Scale applications
* Debug common issues
* Explore containers internally
* Understand self-healing in Kubernetes

---

##  Prerequisites

* Kubernetes cluster (Minikube / Kind / Cloud)
* `kubectl` installed and configured
* Basic knowledge of containers and Kubernetes

---

## 📂 Project Workflow

### 🔹 Step 1: Run a Pod

```bash
kubectl run apache-pod --image=httpd
kubectl get pods
```

---

### 🔹 Step 2: Inspect the Pod

```bash
kubectl describe pod apache-pod
```

Check for:

* Container image
* Port (default: 80)
* Events

---

### 🔹 Step 3: Access the Application

```bash
kubectl port-forward pod/apache-pod 8081:80
```

Open in browser:

```
http://localhost:8081
```

Expected Output: **Apache default page ("It works!")**

---

### 🔹 Step 4: Delete Pod

```bash
kubectl delete pod apache-pod
```

 Insight:

* Pod is permanently deleted
* No automatic recovery (no self-healing)

---

##  Convert to Deployment

### 🔹 Step 5: Create Deployment

```bash
kubectl create deployment apache --image=httpd
kubectl get deployments
kubectl get pods
```

---

### 🔹 Step 6: Expose Deployment

```bash
kubectl expose deployment apache --port=80 --type=NodePort
kubectl port-forward service/apache 8082:80
```

Open:

```
http://localhost:8082
```

---

##  Modify Behavior

### 🔹 Step 7: Scale Deployment

```bash
kubectl scale deployment apache --replicas=2
kubectl get pods
```

 Observation:

* Multiple pods running same application

---

### 🔹 Step 8: Test Load Distribution

* Refresh browser multiple times
* Traffic is distributed across pods

---

##  Debugging Scenario

### 🔹 Step 9: Break the Application

```bash
kubectl set image deployment/apache httpd=wrongimage
kubectl get pods
```

---

### 🔹 Step 10: Diagnose Issue

```bash
kubectl describe pod <pod-name>
```

Look for:

* `ImagePullBackOff`
* Error messages

---

### 🔹 Step 11: Fix the Issue

```bash
kubectl set image deployment/apache httpd=httpd
```

---

##  Explore Inside Container

### 🔹 Step 12: Exec into Pod

```bash
kubectl exec -it <pod-name> -- /bin/bash
```

Inside container:

```bash
ls /usr/local/apache2/htdocs
```

 This directory contains web files.

Exit:

```bash
exit
```

---

##  Self-Healing Feature

### 🔹 Step 13: Delete One Pod

```bash
kubectl delete pod <pod-name>
kubectl get pods -w
```

 Insight:

* Deployment automatically recreates the pod

---

##  Cleanup

```bash
kubectl delete deployment apache
kubectl delete service apache
```

---

##  Key Learnings

* Difference between Pod and Deployment
* How Kubernetes ensures high availability
* Scaling applications dynamically
* Debugging real-world container issues
* Accessing container filesystem
* Understanding self-healing

---

##  Optional Challenge

Modify the web page dynamically:

```bash
kubectl exec -it <pod-name> -- /bin/bash
```

Then:

```bash
echo "Hello from Kubernetes" > /usr/local/apache2/htdocs/index.html
```

Refresh browser to see changes.

---

##  Conclusion

This hands-on exercise provides a complete introduction to running web applications in Kubernetes using Apache. It covers deployment, scaling, debugging, and internal exploration—essential skills for DevOps and cloud engineers.

---