import random

def generate_random_11_digit_number():
    # Generate a random 11-digit number
    return random.randint(10**10, 10**11 - 1)

# Example usage
account_number = generate_random_11_digit_number()
print(f"Random 11-digit number: {account_number}")
