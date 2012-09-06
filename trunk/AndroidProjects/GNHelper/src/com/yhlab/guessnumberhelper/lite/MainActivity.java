package com.yhlab.guessnumberhelper.lite;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.yhlab.guessnumberhelper.guess.Game;

public class MainActivity extends SherlockFragmentActivity implements
        NumberFragment.IGuessedListener, OnSharedPreferenceChangeListener {

    private static final String TAG = "MainActivity";
    private SharedPreferences mSharedPrefs;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        /*
         * register sharedPreferences changed event. If event happened at the
         * time between event unregister and register, the event handler will
         * not be fired. In our case, because the sharedPreferences is changed
         * within another activity which was invoked by this main activity,
         * don't register and unregister event in the onResume and onPause
         * callback method.
         */
        mSharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        mSharedPrefs.registerOnSharedPreferenceChangeListener(this);

        // getSupportActionBar().setDisplayShowTitleEnabled(false);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        /* unregister sharedPreferences changed event */
        mSharedPrefs.unregisterOnSharedPreferenceChangeListener(this);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getSupportMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {
        case R.id.menu_settings:
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        case R.id.menu_restart:

            GNApp app = (GNApp) getApplication();
            app.InitGame(-1, null);
            int firstGuess = app.game.restart();

            FragmentManager fm = getSupportFragmentManager();
            NumberFragment nf = (NumberFragment) fm
                    .findFragmentById(R.id.number_fragment);
            nf.enableAddResult(true);
            nf.setGuessNumber(firstGuess);
            nf.resetResult();

            HistoryFragment hf = (HistoryFragment) fm
                    .findFragmentById(R.id.history_fragment);
            hf.clearHistory();

            return true;

        case R.id.menu_help:
            
            startActivity(new Intent(this, HelpActivity.class));
            return true;
            
        case R.id.menu_about:
        default:
            return super.onOptionsItemSelected(item);
        }
    }

    @Override
    public void onGuess(int number, int result) {
        FragmentManager fm = getSupportFragmentManager();
        HistoryFragment hf = (HistoryFragment) fm
                .findFragmentById(R.id.history_fragment);
        hf.addHistory(number, result);

        GNApp app = (GNApp) getApplication();
        int candidate = app.game.setGuessLabel(number, result);

        switch (candidate) {
        case Game.CANDIDATE_MORE_LUCKY_ONE:
        case Game.CANDIDATE_ONE:
            Log.d(TAG, "Bingo! Got it!");
            /* intent falling down */
        case Game.CANDIDATE_MORE:
            if (candidate != Game.CANDIDATE_MORE_LUCKY_ONE) {
                int nextGuess = app.game.nextGuess();
                NumberFragment nf = (NumberFragment) fm
                        .findFragmentById(R.id.number_fragment);
                nf.setGuessNumber(nextGuess);
            }
            break;
        case Game.CANDIDATE_ZERO:
            Log.d(TAG, "No such number!");
            break;
        }

    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sp, String k) {

        if (k.equals(SettingsActivity.KEY_DIGIT_COUNT)) {

            FragmentManager fm = getSupportFragmentManager();
            NumberFragment nf = (NumberFragment) fm
                    .findFragmentById(R.id.number_fragment);
            nf.changeDigitCount(Integer.parseInt(sp.getString(k, null)));

        } else if (k.equals(SettingsActivity.KEY_GUESS_METHOD)) {

        }
    }
}
