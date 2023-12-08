import PySimpleGUI as sg
import ctypes

# Load the shared library
backend = ctypes.CDLL('./prac.so')  # Adjust the path based on your system

sg.theme('DarkAmber')

layout_signup = [
    [sg.Text("Enter Username: "), sg.Input(key='-USERNAME-')],
    [sg.Text("Enter Password: "), sg.Input(key='-PASSWORD-', password_char='*')],
    [sg.Button('Sign Up'), sg.Button('Cancel')],
]

window_signup = sg.Window("Sign Up", layout_signup)

while True:
    event_signup, values_signup = window_signup.read()

    if event_signup == sg.WIN_CLOSED or event_signup == 'Cancel':
        break
    elif event_signup == 'Sign Up':
        username = values_signup['-USERNAME-']
        password = values_signup['-PASSWORD-']

        # Call the C function from Python
        backend.save_signup_data(ctypes.c_char_p(username.encode('utf-8')),
                                 ctypes.c_char_p(password.encode('utf-8')))

        sg.popup('Sign Up Successful!', title='Success')
print(type(username))
window_signup.close()
