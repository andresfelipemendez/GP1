# Game Programming in C++: Data-Oriented Design and ECS Implementation

Welcome to this repository which showcases practical examples from the book "Game Programming in C++" by Sanjay Madhav. Here, the examples are implemented using two distinct approaches: traditional methods as prescribed in the book and innovative techniques I have adopted to modernize them. The repository follows the book's structure up to Chapter 5. Beyond this, I diverge from the book's outdated use of mesh files in favor of modern GLTF formats, which are more suited to current industry standards.

In my adaptation, I emphasize Data-Oriented Design (DOD) principles to optimize data layout and performance. Additionally, I integrate the Entity Component System (ECS) approach utilizing the ENTT framework, providing a clean, efficient way to manage game entities and state.

This project not only serves as a comprehensive exercise in applying these advanced concepts but also acts as a key portfolio piece to illustrate my capabilities in game development. Notable modifications include the replacement of the SOIL library with stb_image for better support and the use of JSON for scene descriptions, which enhances flexibility and version control compatibility during the game development process.

https://github.com/gameprogcpp/code

## Table of Contents

- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Project Structure](#project-structure)
- [Examples](#examples)
- [Data-Oriented Design](#data-oriented-design)
- [ENTT Framework](#entt-framework)

## Prerequisites

- Visual Studio (version 17.9.6)

## Getting Started

1. Clone the repository:
   ```
   git clone https://github.com/andresfelipemendez/GP1.git
   ```

2. Open the project in Visual Studio.

3. Build the project and run the desired example.

## Project Structure

The repository is organized as follows:

```
game-programming-cpp/
├── chapter-01 oop/
├── chapter-01 dod/
├── chapter-01 ecs/
├── ...
├── libs/
│   ├── SDL2/
│   └── entt/
└── README.md
```

- Each `chapter-N` directory contains implementations according to different paradigms (OOP, DOD, ECS) up to Chapter 5.
- `libs/`: Contains the required libraries (SDL2 and ENTT).

## Examples

The repository includes examples from each chapter of the book. Each example is implemented in two ways:
1. Following the book's original approach.
2. Using a data-oriented design approach with the ENTT framework.

To run an example, navigate to the corresponding directory and build the project using Visual Studio.

## Data-Oriented Design

Data-Oriented Design (DOD) is a programming paradigm that focuses on efficiently organizing data to improve performance. In DOD, data is separated from behavior, and emphasis is placed on optimizing data layout and access patterns.

This repository demonstrates the benefits of this paradigm in game development by converting the examples from the book to a DOD approach.

## ENTT Framework

ENTT is a fast and lightweight Entity Component System (ECS) framework for modern C++. It is designed to be flexible, efficient, and easy to use.

In the DOD implementations of the examples, ENTT is used to streamline the organization of game entities and components in a data-oriented manner.
