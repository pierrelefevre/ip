# Use an official lightweight image with build tools
FROM gcc:latest

# Set up the working directory
WORKDIR /app

# Copy the C server code into the container
COPY server.c .

# Compile the C server
RUN gcc -o server server.c

# Expose the server port
EXPOSE 8080

# Run the server
CMD ["./server"]
