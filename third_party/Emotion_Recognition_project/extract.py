import nbformat

# Load the notebook
notebook_path = "model_improvement (4) (7).ipynb"
with open(notebook_path, 'r', encoding='utf-8') as f:
    nb = nbformat.read(f, as_version=4)

# Extract and print the cells content to understand the structure and details of model3
for cell in nb['cells']:
    if cell['cell_type'] == 'code':
        print(cell['source'])
        print("\n---\n")  # Separator between cells