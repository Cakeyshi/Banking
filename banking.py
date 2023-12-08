import PySimpleGUI as sg
import pycountry
import ctypes
import random

banking = ctypes.CDLL('./banking.so')

# design
def main():
    sg.theme('LightPurple')
    layout = [
        [sg.Button("Login", key='-LOGIN-'), sg.Button("Sign Up", key='-SIGNUP-')],
        [sg.Button("Exit", key='-EXIT-')]
    ]

    window = sg.Window("My Bank", layout, resizable=True)

    while True:
        event, values = window.read()

        if event in (sg.WIN_CLOSED, '-EXIT-'):
            break
        elif event == '-SIGNUP-':
            country_names = [country.name for country in pycountry.countries]
            layout_signup = [
                [sg.Text("First Name: "), sg.Input(key='-FIRSTNAME-'), sg.Text("Last Name: "), sg.Input(key='-LASTNAME-'), sg.Text("Middle Name: "), sg.Input(key='-MIDDLENAME-')],
                [sg.Text("Birthday: ")],
                [sg.Text("Day: "), sg.Input(key='-BIRTHDATE-'), sg.Text("Month: "), sg.Input(key='-BIRTHMONTH-'), sg.Text("Year: "), sg.Input(key='-BIRTHYEAR-')],
                [sg.Text("Select Country: ")],
                [sg.Combo(country_names, key='-COUNTRY-', default_value='Philippines')],
                [sg.Text("Phone Number: "), sg.Input(key='-PNUMBER-')],
                [sg.Text("Pin Number: "), sg.Input(key='-PIN-')],
                [sg.Button("Submit", key='-SUBMIT-')]
            ]

            window_signup = sg.Window("Sign Up", layout_signup, resizable=True)

            while True:
                event_signup, values_signup = window_signup.read()

                if event_signup == sg.WIN_CLOSED:
                    break

                elif event_signup == '-SUBMIT-':
                    # Call the C function from the shared library
                    account_number = generate_random_11_digit_number()

                    try:
                       banking.signUp(
                            values_signup['-FIRSTNAME-'], values_signup['-LASTNAME-'],
                            values_signup['-MIDDLENAME-'], int(values_signup['-BIRTHDATE-']),
                            int(values_signup['-BIRTHMONTH-']), int(values_signup['-BIRTHYEAR-']),
                            values_signup['-COUNTRY-'], str(values_signup['-PNUMBER-']),
                            str(account_number), str(values_signup['-PIN-'])
                        )

                    except Exception as e:
                        sg.popup(f"Error: {e}", title="Error", button_color=('white', 'green'), custom_text='OK')
                        print(f"Error: {e}")
                    window_signup.close()
    window.close()

def generate_random_11_digit_number():
 
    return random.randint(10**10, 10**11 - 1)

if __name__ == '__main__':
    main()
