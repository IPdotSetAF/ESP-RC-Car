#!/bin/bash

SOURCE_FOLDER="../www"
DESTINATION_FOLDER="../data/www"

# Clean destination folder before copying new files
rm -rf "$DESTINATION_FOLDER"
mkdir -p "$DESTINATION_FOLDER"

# Function to recursively copy files and compare sizes
process_folder() {
    local source="$1"
    local destination="$2"

    for source_file in "$source"/*; do
        if [ -d "$source_file" ]; then
            local relative_path="${source_file#$SOURCE_FOLDER}"
            mkdir -p "$destination$relative_path"
            process_folder "$source_file" "$destination"
        elif [ -f "$source_file" ]; then
            filename=$(basename "$source_file")
            original_size=$(stat -c %s "$source_file")

            # Compress the file using gzip
            gzip -c "$source_file" > "$destination${source_file#$SOURCE_FOLDER}.gz.tmp"
            compressed_size=$(stat -c %s "$destination${source_file#$SOURCE_FOLDER}.gz.tmp")

            # Compare sizes and determine which to copy
            if [ "$compressed_size" -lt "$original_size" ]; then
                # If compressed version is smaller, move to destination with .gzip extension
                mv "$destination${source_file#$SOURCE_FOLDER}.gz.tmp" "$destination${source_file#$SOURCE_FOLDER}.gz"
                echo "Compressed and copied" "$destination${source_file#$SOURCE_FOLDER}.gz"
            else
                # If original is smaller or same size, move to destination without compression
                cp "$source_file" "$destination${source_file#$SOURCE_FOLDER}"
                echo "Copied" "$destination${source_file#$SOURCE_FOLDER}"
            fi

            # Clean up compressed file
            rm -f "$destination${source_file#$SOURCE_FOLDER}.gz.tmp"
        fi
    done
}

# Start recursive processing
process_folder "$SOURCE_FOLDER" "$DESTINATION_FOLDER"

echo "All files processed."
