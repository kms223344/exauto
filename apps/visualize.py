from graphviz import Digraph
import os
import getopt
import sys

# Note: https://chatgpt.com/share/68038a27-17c8-800a-ba93-585f59f7bb52

size_t = 8
FONT = 'Consolas'
path = None
output = None


def parse_args():
    def help():
        print(
            """
    Usage: python visualize.py [options]
    Options:
        -h, --help          Show this help message and exit
        --path=PATH         Path to the DFA file
        --output=OUTPUT     Output file name (without extension)
        --font=FONT        Font name for the nodes (default: Consolas)
            """
        )
    global path, output, FONT
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help", "path=", "output=", "font="])
    except getopt.GetoptError as err:
        print(err)
        sys.exit(1)

    for o, a in opts:
        if o in ("-h", "--help"):
            help()
            sys.exit(0)
        elif o in ("--path"):
            if a:
                path = a 
                if not os.path.exists(path):
                    print("File not found.")
                    sys.exit(1)
            else:
                help()
                sys.exit(1)
        elif o in ("--output"):
            if a:
                output = a
            else:
                help()
                sys.exit(1)
        elif o in ("--font"):
            if a:
                FONT = a
            else:
                help()
                sys.exit(1)
        else:
            assert False, "Unhandled option"

def get_DFA(path: str):
    f = open(path, 'rb')
    # StateConf
    nState = int.from_bytes(f.read(size_t), 'little')
    q0 = int.from_bytes(f.read(size_t), 'little')
    n = int.from_bytes(f.read(size_t), 'little')
    endStates = []
    for i in range(n):
        endStates.append(int.from_bytes(f.read(size_t), 'little'))
    
    # InputConf
    dummy = int.from_bytes(f.read(size_t), 'little')
    nInput = int.from_bytes(f.read(size_t), 'little')

    mapping = {} # [ c -> i ]
    for i in range(256):
        c = int.from_bytes(f.read(size_t), 'little')
        if c == 18446744073709551615: # -1
            pass
        else:
            mapping[c] = chr(i)

    # Transition
    n = int.from_bytes(f.read(size_t), 'little')
    transition = {} # [ p -(c)-> q ]
    for i in range(n):
        c = int.from_bytes(f.read(size_t), 'little')
        m = int.from_bytes(f.read(size_t), 'little')
        transition[c] = {}
        for j in range(m):
            p = int.from_bytes(f.read(size_t), 'little')
            q = int.from_bytes(f.read(size_t), 'little')
            transition[c][p] = q

    f.close()
    return {
        'StateConf': {
            'nState': nState,
            'q0': q0,
            'endStates': endStates
        },
        'InputConf': {
            'nInput': nInput,
            'mapping': mapping
        },
        'Transition': transition
    }

    
def main():
    parse_args()
    global path, output
    if path == None:
        path = input("Enter the path to the DFA file: ")
    if not os.path.exists(path):
        print("File not found.")
        return
    dfa = get_DFA(path)
    print("DFA Configuration:")
    print(dfa)
    # Visualize the DFA
    dot = Digraph()
    for state in range(dfa['StateConf']['nState']):
        if(state == dfa['StateConf']['q0']):
            dot.node(str(state), str(state), shape='circle', style='filled', fillcolor='lightblue', fontname = FONT)
        elif(state in dfa['StateConf']['endStates']):
            dot.node(str(state), str(state), shape='doublecircle', fontname = FONT)
        else:
            dot.node(str(state), str(state), shape='circle', fontname = FONT)
    for c in dfa['Transition']:
        for p in dfa['Transition'][c]:
            q = dfa['Transition'][c][p]
            dot.edge(str(c), str(q), label=dfa['InputConf']['mapping'][p], fontname = FONT)
    if(output == None):
        output = os.path.basename(path)
    dot.render(f'rendered/{output}', view=False, format='svg')

if __name__ == '__main__':
    main()
