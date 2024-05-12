# Game Programming in C++ - Data-Oriented Design Implementation

This repository contains the examples from the book "Game Programming in C++" by Sanjay Madhav, implemented in two different ways:

Following the book's original examples
Converting the examples to a data-oriented design (DOD) approach using the ENTT framework

This project serves as an exercise and a portfolio piece to demonstrate the application of data-oriented design principles in game development.

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
├── book-examples/
│   ├── chapter-01/
│   ├── chapter-02/
│   └── ...
├── dod-examples/
│   ├── chapter-01/
│   ├── chapter-02/
│   └── ...
├── libs/
│   ├── SDL2/
│   └── entt/
└── README.md
```

- `book-examples/`: Contains the original examples from the book, organized by chapter.
- `dod-examples/`: Contains the data-oriented design implementations of the examples, organized by chapter.
- `libs/`: Contains the required libraries (SDL2 and ENTT).

## Examples

The repository includes examples from each chapter of the book. Each example is implemented in two ways:
1. Following the book's original approach
2. Using a data-oriented design approach with the ENTT framework

To run an example, navigate to the corresponding directory and build the project using Visual Studio.

## Data-Oriented Design

Data-oriented design (DOD) is a programming paradigm that focuses on organizing data efficiently to improve performance. In DOD, data is separated from behavior, and the emphasis is placed on optimizing data layout and access patterns.

In this repository, the examples from the book are converted to a DOD approach to demonstrate the benefits of this paradigm in game development.

## ENTT Framework

ENTT is a fast and lightweight entity-component-system (ECS) framework for modern C++. It is designed to be flexible, efficient, and easy to use.

In the DOD implementations of the examples, ENTT is used to avoid implementing a custom ECS from scratch. ENTT provides a solid foundation for organizing game entities and components in a data-oriented manner.
