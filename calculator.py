import PySimpleGUI as sg
import ctypes

# Load the shared library
calculator = ctypes.CDLL('./calculator.so')

# PySimpleGUI layout
sg.theme('DarkAmber')
layout = [
    [sg.Input(size=(10, 1), key='-INPUT-'), sg.Button('1'), sg.Button('2'), sg.Button('3')],
    [sg.Input(size=(10, 1), key='-RESULT-')],
    [sg.Button('Add', bind_return_key=True), sg.Button('Exit')]
]

window = sg.Window('Calculator', layout)

while True:
    event, values = window.read()

    if event in (sg.WIN_CLOSED, 'Exit'):
        break

    if event == 'Add':
        try:
            # Call the add function from the C library
            result = calculator.add(int(values['-INPUT-']), 42)
            window['-RESULT-'].update(result)
        except Exception as e:
            print(f"Error: {e}")

window.close()