patnames = ['hatclosed', 'hatopen', 'rim', 'shaker', 'cymbal', 'kick', 'snare', 'clap', 'cowbell', 'claves', 'congahigh', 'congamid', 'congalow']

for number in range(1,17):
    for item in patnames:
        half_string = 'const word ' + item + '_p' +str(number) + ' PROGMEM '
        print(f'{half_string:33}= 0b0000000000000000;')

    assemble_string = 'const word pattern' + str(number) + '[] PROGMEM = {'
    for idx, item in enumerate(patnames):
        if idx < 12:
            assemble_string += item + '_p' + str(number) + ', '
        else:
            assemble_string += item + '_p' + str(number) + '};'

    print(assemble_string)
    print()