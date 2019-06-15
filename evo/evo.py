import numpy as np
import cv2
import matplotlib.pyplot as plt

def main(factor_T):
    del_line = '\r\x1b[K'

    SIDE = 200
    GENERATIONS = 200
    board = np.random.rand(SIDE, SIDE)
    scores = np.zeros((SIDE, SIDE))

    # factor_T = 1.62 # defector vs cooperator
    factor_R = 1 # cooperator vs cooperator
    factor_P = 0 # defector vs defector
    factor_S = 0 # cooperator vs defector

    # 90% cooperators (True) and 10% defectors (False)
    board = board > 0.1
    board_prev = board.copy()

    np.random.seed(2333)

    def index_valid(index, board):
        return (0 <= index[0] < board.shape[0] 
            and 0 <= index[1] < board.shape[1])


    def play(times=1):
        for t in range(times):
            # print(board)
            print(f'{del_line}{t + 1} / {times}', end='')
            board_prev = board.copy()
            scores = np.zeros((SIDE, SIDE))

            it = np.nditer(board, flags=['multi_index'])
            while not it.finished:
                owner = it.multi_index
                owner_cooperate = it[0]
                
                # print(f'\nowner: {repr(owner)} {owner_cooperate}')

                for offset in [(0, 1), (1, -1), (1, 0), (1, 1)]:
                    guest = (owner[0] + offset[0], 
                        owner[1] + offset[1])
                    # print(f'guest: {repr(guest)}', end=' ')
                    if not index_valid(guest, board):
                        # print('guest invalid')
                        continue
                    else:
                        # print(f'{board[guest]}')
                        pass


                    if board[owner] and board[guest]:
                        # print('both +1')
                        scores[owner] += factor_R
                        scores[guest] += factor_R
                    elif board[owner] and not board[guest]:
                        # print('owner +0, guest +b')
                        scores[owner] += factor_S
                        scores[guest] += factor_T
                    elif not board[owner] and board[guest]:
                        # print('owner +b, guest +0')
                        scores[owner] += factor_T
                        scores[guest] += factor_S
                    else:
                        # print('owner +0, guest +0')
                        scores[owner] += factor_P
                        scores[guest] += factor_P

                it.iternext()

            # print(scores)

            it = np.nditer(scores, flags=['multi_index'])
            while not it.finished:
                lattice = it.multi_index

                candidates = [(lattice[0] + i, lattice[1] + j) 
                    for i in range(-1, 2) for j in range(-1, 2)]

                candidates_score = [(i, j, scores[i, j]) 
                    for i, j in candidates 
                    if index_valid((i, j), scores)]

                winner = max(candidates_score, key=lambda x: x[2])
                board[lattice] = board[winner[:2]]
                it.iternext()

            


    def show():
        blue = [51, 51, 255] # C -> C
        red = [255, 51, 51] # D -> D
        yellow = [255, 255, 51] # C -> D
        green = [51, 255, 51] # D -> C

        pic = np.zeros((SIDE, SIDE, 3), dtype=np.uint8)
        for i in range(SIDE): 
            for j in range(SIDE):
                # raise Exception
                if board_prev[i, j] and board[i, j]:
                    pic[i, j] = blue
                elif not board_prev[i, j] and not board[i, j]:
                    pic[i, j] = red
                elif board_prev[i, j] and not board[i, j]:
                    pic[i, j] = yellow
                else:
                    pic[i, j] = green

        # raise Exception

        plt.imshow(pic, interpolation='nearest', aspect='auto')
        plt.axis('off')
        plt.savefig(f'evo_{factor_T}.png')
        plt.savefig(f'evo_{factor_T}.eps', format='eps')

    play(200)
    show()

if __name__ == '__main__':
    for i in range(1, 5):
        main(1.58 + i * 0.08)