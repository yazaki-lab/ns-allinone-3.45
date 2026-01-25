import csv
import random

def generate_random_params(num_patterns=10):
    """
    ランダムなパラメータセットを生成
    
    Parameters:
    - num_patterns: 生成するパターン数（デフォルト: 10）
    
    Returns:
    - パラメータのリスト
    """
    params = []
    
    for i in range(1, num_patterns + 1):
        radius = round(random.uniform(1.0, 50.0), 1)
        nStations = random.randint(1, 50)
        heavyUserRate = random.randint(5, 30)
        
        params.append({
            'id': i,
            'radius': radius,
            'nStations': nStations,
            'heavyUserRate': heavyUserRate
        })
    
    return params

def save_to_csv(params, filename='ns3_params.csv'):
    """
    パラメータをCSVファイルに保存
    
    Parameters:
    - params: パラメータのリスト
    - filename: 出力ファイル名
    """
    with open(filename, 'w', newline='', encoding='utf-8') as f:
        fieldnames = ['id', 'radius', 'nStations', 'heavyUserRate']
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        
        writer.writeheader()
        writer.writerows(params)
    
    print(f"✓ {filename} に {len(params)} パターン保存しました")

def print_params(params):
    """パラメータを表形式で表示"""
    print("\n生成されたパラメータ:")
    print("-" * 60)
    print(f"{'ID':<5} {'radius':<10} {'nStations':<12} {'heavyUserRate':<15}")
    print("-" * 60)
    
    for p in params:
        print(f"{p['id']:<5} {p['radius']:<10} {p['nStations']:<12} {p['heavyUserRate']:<15}")
    
    print("-" * 60)

if __name__ == "__main__":
    # パラメータ数を設定（必要に応じて変更）
    NUM_PATTERNS = 10
    
    # ランダムパラメータを生成
    params = generate_random_params(NUM_PATTERNS)
    
    # コンソールに表示
    print_params(params)
    
    # CSVに保存
    save_to_csv(params, 'ns3_params.csv')
    
    print("\n実行完了")