#!/bin/bash

# Set the input and output directories
GLSL_DIR="glsl"
BINARY_DIR="binary"

# Create the binary directory if it doesn't exist
mkdir -p "$BINARY_DIR"

# Loop through all .vs.glsl and .fs.glsl files in the glsl directory
for shader in "$GLSL_DIR"/*.vs.glsl "$GLSL_DIR"/*.fs.glsl; do
    # Check if the file exists (in case of no matches)
    if [ -f "$shader" ]; then
        # Extract the filename without the directory and extension
        shader_name=$(basename "$shader" .glsl)

        # Replace '.' with '_' in the shader name for the variable name
        var_name="${shader_name//./_}"

        # Set the output header file path in the binary directory
        output_header="$BINARY_DIR/$shader_name.h"

        # Read the contents of the shader file
        shader_text=$(<"$shader")

        # Prepare the content for the header file
        header_content="#ifndef ${var_name^^}_H\n"
        header_content+="#define ${var_name^^}_H\n\n"
        header_content+="#include <string_view>\n\n"
        header_content+="const std::string_view ${var_name} = R\"("
        header_content+="$shader_text"
        header_content+=")\";\n\n"
        header_content+="#endif // ${var_name^^}_H"

        # Write the modified content to the output header file
        echo -e "$header_content" > "$output_header"

        echo "Generated header file: $output_header"
    else
        echo "No shader files found."
    fi
done
