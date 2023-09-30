# StickSkeletonV2 - OpenGL Humanoid Animation Project

StickSkeletonV2 is an OpenGL project featuring a humanoid character created from basic geometric shapes. The project offers various animation modes, which you can select by changing a single configuration setting.

## Features

- **Humanoid Figure**: StickSkeletonV2 showcases a humanoid character constructed from simple geometric objects.

- **Flexible Animation**: You can choose from different animation modes by adjusting a single configuration setting, making it easy to explore various animation techniques.

- **CSV Animation**: The project includes CSV-based animations, allowing for realistic movements of the humanoid character.

## Getting Started

To get started with StickSkeletonV2:

1. **Clone the Repository**: Clone this GitHub repository to your local machine.

2. **Configuration**: In the `utilities.h` file, you can change the animation mode by modifying the `#define animationSequence <sequence_name>` line. Options include:
   - `walking`: Uses code calculations for animation.
   - `csv_walking`: Reads animations inefficiently from CSV files.
   - `csv_walking_efficient`: Employs an efficient CSV reading method for animations.

3. **Build and Run**: Build the project and run the application to observe the humanoid character's animations based on your chosen mode.

4. **Editing Mode**: In the `StickSkeletonV2.cpp` file, you can set the `#define EDITING` configuration to `true` to pause animations and access editing modes for further development.


## License

This project is open-source and available under the MIT License. See the [LICENSE](LICENSE) file for details.


Enjoy experimenting with animations and graphics!
