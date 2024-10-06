### Annotation description

You will be given an human-written sentence, which describes an experiment in a NLP paper, along with a model generated explanation that tries to explain the motivation of this experiment. 

Your task is to decide whether the model-generated explanation is acceptable or not (i.e., sounds reasonble, instead of hallucination). Enter `0` in the terminal if you think the explanation is not acceptable, and `1` if you think the explanation is acceptable.

### Run annotation

no specific python package is required. You can run in your terminal by:
```bash
python annotate.py
```

you will see something printed in the terminal like:
```
*
*
*
*
*

==================== ### Experiment Idea ### ====================
```
1. SimCSE performance evaluation: 
```

==================== ### Model Explanation ### ====================
==> model_name:  model A
```
1. To demonstrate the effectiveness of SimCSE by providing a direct performance comparison against existing state-of-the-art methods on widely recognized benchmarks.
```

==================== ### Annotation ### ====================
==> is this model's explanation sounds reasonable and acceptable? (enter '0' if not acceptable, '1' if acceptable)
```

you have to enter in the terminal either `0` or `1` based on your judgement. 

Note that, there are 5 models' generated explanations, which means you will see one experiment idea repeat for 5 times along with 5 different model explanations (model A, B, C, D, E).  

It's possible, model's generated explanation is `None` or just an empyt string `""`, just give `0` in that case.

Once you start annotating, pls DO NOT close your terminal, otherwise you will lose the progress.
