package com.yhlab.guessnumberhelper.lite;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TableRow;
import android.widget.Toast;
import com.actionbarsherlock.app.SherlockFragment;
import com.yhlab.component.guessnumber.GuessedNumber;
import com.yhlab.component.guessnumber.GuessedResult;

public class NumberFragment extends SherlockFragment
        implements View.OnClickListener {

    @SuppressWarnings("unused")
    private static final String TAG = "NumberFragment";
    private GuessedNumber gn;
    private GuessedResult gr;
    private Button btnAdd;
    private int digitCount;

    private final String KEY_NUMBER = "Number";
    private final String KEY_RESULT = "Result";
    private final String KEY_BTN_ADD = "BtnAdd";

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        try {
            mGuessedListener = (IGuessedListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement IGuessedListener interface.");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        View v = inflater.inflate(R.layout.fragment_number, container, false);

        SharedPreferences sp = PreferenceManager
                .getDefaultSharedPreferences(getActivity());
        digitCount = Integer.parseInt(sp.getString(
                SettingsActivity.KEY_DIGIT_COUNT, "4"));

        /* add dynamic widgets GuessedNumber */
        gn = new GuessedNumber(getActivity(), digitCount, "0123456789?");
        gn.setId(R.id.guessed_number);
        TableRow tr = (TableRow) v.findViewById(R.id.row_guess);
        TableRow.LayoutParams trlp = new TableRow.LayoutParams();
        trlp.column = 1;
        trlp.span = 2;
        tr.addView(gn, trlp);

        /* add dynamic widgets GuessedResult */
        gr = new GuessedResult(getActivity(), digitCount);
        gr.setId(R.id.guessed_result);
        tr = (TableRow) v.findViewById(R.id.row_result);
        tr.addView(gr, 1);

        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        btnAdd = (Button) (getView().findViewById(R.id.btn_result_add));
        btnAdd.setOnClickListener(this);

        /* check if we need to restore previous instance state */
        if (savedInstanceState != null) {
            gn.setNumber(savedInstanceState.getInt(KEY_NUMBER));
            gr.setResult(savedInstanceState.getInt(KEY_RESULT));
            btnAdd.setEnabled(savedInstanceState.getBoolean(KEY_BTN_ADD));
        }
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        /* save wheel state */
        outState.putInt(KEY_NUMBER, gn.getNumber());
        outState.putInt(KEY_RESULT, gr.getResult());
        outState.putBoolean(KEY_BTN_ADD, btnAdd.isEnabled());
    }

    @Override
    public void onClick(View v) {

        int id = v.getId();

        switch (id) {
        case R.id.btn_result_add:
            int gnn = gn.getNumber();
            int grr = gr.getResult();
            int grrA = (grr >> 4) & 0xF;
            int grrB = grr & 0xF;
            boolean invalidNumber = false;
            boolean invalidResult = false;

            for (int digit = 0; digit < digitCount; digit++) {
                if (((gnn >> (digit * 4)) & 0xF) >= 10) {
                    invalidNumber = true;
                    break;
                }
            }
            if (grrA > digitCount || grrB > digitCount
                    || (grrA + grrB) > digitCount
                    || (grrA == digitCount - 1 && grrB == 1)) {
                invalidResult = true;
            }

            if (invalidNumber)
                Toast.makeText(getActivity().getApplicationContext(),
                        getString(R.string.notify_badformat_guess),
                        Toast.LENGTH_SHORT).show();
            else if (invalidResult)
                Toast.makeText(getActivity().getApplicationContext(),
                        getString(R.string.notify_badformat_result),
                        Toast.LENGTH_SHORT).show();
            else
                mGuessedListener.onGuess(gnn, grr);
            break;
        default:
            break;
        }
    }

    public interface IGuessedListener {
        public void onGuess(int number, int result);
    }

    private IGuessedListener mGuessedListener;

    public void changeDigitCount(int newDigitCount) {
        gn.changeDigitCount(newDigitCount);
        gr.changeDigitUsed(newDigitCount);
        
        // remember to update the cached digitCount, this function is called
        // due to the game setting changes.
        digitCount = newDigitCount;
    }

    public void setGuessNumber(int guess) {
        gn.setNumber(guess, true);
    }

    public int getGuessNumber() {
        return gn.getNumber();
    }

    public void resetResult() {
        gr.resetToUnknown();
    }

    public void enableAddResult(boolean enabled) {
        Button btn = (Button) getView().findViewById(R.id.btn_result_add);
        if (btn != null) {
            btn.setEnabled(enabled);
        }
    }

}
