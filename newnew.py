from ctypes import CDLL, c_longlong

# Load the shared library
random_generator = CDLL('./random.so')

# Call the C function from the shared library
generate_random_11_digit_number = random_generator.generateRandom11DigitNumber
generate_random_11_digit_number.restype = c_longlong  # Specify the return type
result = generate_random_11_digit_number()

# Print the result
print(f"Random 11-digit number: {result}")

