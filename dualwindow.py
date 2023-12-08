import PySimpleGUI as sg

def main():
    sg.theme('LightPurple')

    # Layout for the input window
    layout_input = [
        [sg.Text('Enter something:'), sg.InputText(key='-INPUT-')],
        [sg.Button('Submit')]
    ]

    window_input = sg.Window('Input Window', layout_input)

    # Event loop for the input window
    while True:
        event_input, values_input = window_input.read()

        if event_input == sg.WINDOW_CLOSED:
            break
        elif event_input == 'Submit':
            user_input = values_input['-INPUT-']
            window_input.close()
            show_output_window(user_input)

    # Close the input window
    window_input.close()

def show_output_window(user_input):
    # Layout for the output window
    layout_output = [
        [sg.Text(f'You entered: {user_input}')],
        [sg.Button('OK')]
    ]

    window_output = sg.Window('Output Window', layout_output)

    # Event loop for the output window
    while True:
        event_output, values_output = window_output.read()

        if event_output == sg.WINDOW_CLOSED or event_output == 'OK':
            break

    # Close the output window
    window_output.close()

if __name__ == '__main__':
    main()
