import gdb

class VisualizaGNL(gdb.Command):
    """
    Comando para desenhar a t_gnl_list e seus chunks.
    Uso: gnl <nome_da_variavel_lista>
    Exemplo: gnl list
             gnl &contexts[0]
    """

    def __init__(self):
        super(VisualizaGNL, self).__init__("gnl", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        if not arg:
            print("\n Erro: Passe a variavel da lista como argumento. Ex: gnl list")
            return

        try:
            gnl_list_val = gdb.parse_and_eval(arg)

            if gnl_list_val.type.code == gdb.TYPE_CODE_PTR:
                if int(gnl_list_val) == 0:
                    print("\n[ A lista passada eh NULL ]\n")
                    return
                gnl_list_val = gnl_list_val.dereference()

            head = gnl_list_val['head']
            tail = gnl_list_val['tail']

            print("\n" + "="*50)
            print("VISUALIZADOR GET_NEXT_LINE")
            print("="*50)

            if int(head) == 0:
                print("\n[ Lista Vazia - Head eh NULL ]\n")
                print("="*50 + "\n")
                return

            print(f"Head Pointer: {head}")
            print(f"Tail Pointer: {tail}")
            print("-" * 50)

            current = head
            node_count = 0
            diagrama = "\n(HEAD) -> "

            while int(current) != 0:
                node_count += 1
                content_ptr = current['content']
                
                if int(content_ptr) != 0:
                    try:
                        content_str = content_ptr.string()
                        display_str = content_str.replace('\n', '\\n')
                    except gdb.MemoryError:
                        display_str = "ERRO DE MEMÓRIA"
                else:
                    display_str = "NULL"

                box = f'[{display_str}]'

                if int(current) == int(tail):
                    box += " <- (TAIL)"

                diagrama += box

                next_node = current['next']
                if int(next_node) != 0:
                    diagrama += "\n          |\n          V\n          "
                
                current = next_node

            print(diagrama)
            print(f"\nTotal de chunks (nós) na memória: {node_count}")
            print("="*50 + "\n")

        except gdb.error as e:
            print(f"\n Erro do GDB: {e}")
            print("Verifique se o argumento passado é realmente do tipo t_gnl_list ou t_gnl_list*.\n")

VisualizaGNL()